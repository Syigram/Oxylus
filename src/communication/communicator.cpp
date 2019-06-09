#include <iostream>
#include <cmath>

#include <oxylus/communication/communicator.h>

#include <oxylus/training/vectors_generator.h>
#include <oxylus/memory/file_reader.h>
#include <oxylus/training/trainer.h>

using namespace rdf::bpc;

using namespace boost::archive;

Communicator::Communicator(){
  this->rank = world.rank();
  this->size = world.size();
}

Communicator::Communicator(ConfigurationObject* configObject){
  this->rank = world.rank();
  this->size = world.size();
  this->configObject = configObject;
  this->featuresSize = configObject->GetFeaturesSize();
  this->thresholdsSize = configObject->GetThresholdsSize();
}

int Communicator::GetRank(){
  return this->rank;
}

void Communicator::AssignImagesToEachNode(){
  this->GatherMemoryInformationMessage();
  ImageBatchMessage imageBatchForScatter;
  this->ScatterImagesBatchMessage(imageBatchForScatter);
  this->imageBatchMessage.Print();
}

void Communicator::LoadImagesToStructures(){
  int indexStart = this->imageBatchMessage.GetIndexStart();
  int indexEnd = this->imageBatchMessage.GetIndexEnd();
  rdf::bpc::FileReader reader(this->configObject);
  this->imagesStructureVector = reader.ReadImages(indexStart, indexEnd);
  AssignTreeBitsToImages(imagesStructureVector);
}

void Communicator::GatherMemoryInformationMessage(){
  MemoryMessage memoryMessage(this->configObject, rank);
  memoryMessage.CreateMemoryMessage();
  memoryMessage.Print();
  mpi::gather(world, memoryMessage, this->memoryMessageVec, MPI_MASTER);
  this->MasterPrint("gathering memory messages..");
}

void Communicator::AssignTreeBitsToImages(std::shared_ptr<ImagesVector> imagesVec) {
  ImageOperations imageOperations(this->configObject);
  imageOperations.AssignImagesToTrees(imagesVec);
}


void Communicator::BeginTraining(){
  int numOfTrees = this->configObject->GetNumberOfTrees();
  int currentNode = imagesStructureVector->at(0).pointsVector->at(0).GetCurrentNode();
  int minPointsPerNode = 2000;
  int bestThresholdIndex = 0;
  int bestFeatureIndex = 0;
  Trainer trainer(this->configObject);
  std::vector<PartialTrainedNode> vecPartialNodes;
  std::vector<NodeVectors> gatheredNodeVectors;
  std::vector<Matrix<Cell>> vecNodeHistograms;
  /* for (int i = 0; i < numOfTrees; i++) { */
    int pointsPerNode = ImageOperations::GetNumberOfPointsForNode(currentNode, imagesStructureVector);
    if (pointsPerNode < minPointsPerNode){
      std::cout << "Not enough points for node: " << currentNode << std::endl;
    }
    NodeVectors nodeVectors = this->BroadcastNodeVectors(currentNode);
    /* std::vector<int>* thresholdsVec = new std::vector<int>(); */
    /* std::vector<Features>* featuresVec = new std::vector<Features>(); */
    /* thresholdsVec->reserve(configObject->GetThresholdsSize()); */
    /* featuresVec->reserve(configObject->GetFeaturesSize()); */
    /* thresholdsVec = nodeVectors.thresholdsVec; */
    /* featuresVec = nodeVectors.featuresVec; */
    /* nodeVectors.featuresVec = featuresVec; */
    /* nodeVectors.thresholdsVec = thresholdsVec; */
    /* trainer.TrainNode(imagesStructureVector, nodeVectors); */
    trainer.TrainNode(imagesStructureVector, nodeVectors);
    /* Matrix<Cell> nodeHisto =  trainer.TrainNode(imagesStructureVector, nodeVectors); */
    mpi::gather(world, nodeVectors, gatheredNodeVectors, MPI_MASTER);
    /* this block reduces the histograms, calculates the argmin value and sends
     * the indexes of the best feature and threshold back to the slave nodes
     * so that they can do the processing of images using these best features found */
    if (rank == MPI_MASTER) {
      Matrix<Cell> reducedHistograms = ReduceHistograms(gatheredNodeVectors);
      UpdateHistogramsCount(reducedHistograms);
      std::pair<int, int> bestFeatureAndThreshold = CalculateArgMinValues(reducedHistograms);
      bestFeatureIndex = std::get<0>(bestFeatureAndThreshold);
      bestThresholdIndex = std::get<1>(bestFeatureAndThreshold);
      mpi::broadcast(world, bestThresholdIndex, MPI_MASTER);
      mpi::broadcast(world, bestFeatureIndex, MPI_MASTER);
    }
    world.barrier();

    WeakLearnerNode* weakLearnerNode = trainer.CreateTrainedNode(currentNode,
                      bestFeatureIndex, bestThresholdIndex, nodeVectors);
    trainer.EvaluateImages(imagesStructureVector, weakLearnerNode);


    /* std::cout << "survibed" << std::endl; */
  /* } */
}


rdf::Matrix<Cell> Communicator::ReduceHistograms(std::vector<NodeVectors> &gatheredNodeVectors) {
  Cell myCell;
  Matrix<Cell> histogramsAccumulated(featuresSize, thresholdsSize, myCell);
  for (auto& nodeVector: gatheredNodeVectors) {
    for (int i = 0; i < featuresSize; i++) {
      for (int j = 0; j < thresholdsSize; j++) {
        histogramsAccumulated[i][j].Reduce(nodeVector.nodeHistograms[i][j]);
      }
    }
  }
  return histogramsAccumulated;
}


void Communicator::UpdateHistogramsCount(Matrix<Cell>& nodeHistograms) {
  for (int i = 0; i < featuresSize; i++) {
    for (int j = 0; j < thresholdsSize; j++) {
      nodeHistograms[i][j].SetHistogramsTotalCount();
    }
  }
}


NodeVectors Communicator::BroadcastNodeVectors(int nodeId){
  NodeVectors nodeVectors;
  if (rank == MPI_MASTER) {
    MasterPrint("am master");
    VectorsGenerator vectorGenerator(this->configObject);
    std::vector<int>* thresholdsVec = vectorGenerator.GenerateThresholdsVector();
    std::vector<Features>* featuresVec = vectorGenerator.GenerateFeaturesVector();
    nodeVectors.nodeId = nodeId;
    nodeVectors.featuresVec = featuresVec;
    nodeVectors.thresholdsVec = thresholdsVec;
  } 
  mpi::broadcast(world, nodeVectors, MPI_MASTER);
  return nodeVectors;
}

void Communicator::ScatterImagesBatchMessage(ImageBatchMessage newImageBatchMessage){
  ImageBatchMessageVec imageBatchMessageVec;
  int newBatchLimit = 2;
  int i = 0;
  for(auto &memoryMessage: this->memoryMessageVec){
    /* int batchSize = memoryMessage.GetBatchSize(); */ /* TODO: use real batch size calc */
    int batchSize = 4;
    int start = newBatchLimit;
    int end = newBatchLimit + batchSize;
    newImageBatchMessage.SetBatchSize(batchSize);
    newImageBatchMessage.SetIndexStart(start);
    newImageBatchMessage.SetIndexEnd(end);
    newImageBatchMessage.SetIdProcess(i);
    imageBatchMessageVec.push_back(newImageBatchMessage);
    newBatchLimit = end + 1;
    i++;
  }
  mpi::scatter(world, imageBatchMessageVec, this->imageBatchMessage, MPI_MASTER);
}


std::pair<Features, int> Communicator::FindLowestArgMin() {


}


double Communicator::GetArgMinValue(Cell& cell) {
  double S_L_Magnitude = (double) cell.leftHistogramTotal;
  double S_R_Magnitude = (double) cell.rightHistogramTotal;
  double S_Magnitude = (double) cell.totalCount;
  double I_L = ClassificationObjectiveFunction(cell.leftHistogram, S_L_Magnitude);
  double I_R = ClassificationObjectiveFunction(cell.rightHistogram, S_R_Magnitude);
  double SL_BY_IL = (S_L_Magnitude / S_Magnitude) * I_L;
  double SR_BY_IR = (S_R_Magnitude / S_Magnitude) * I_R;
  return SL_BY_IL + SR_BY_IR;
  /* std::cout << "argMin value: " << cell.argMinValue << std::endl; */
}

double Communicator::ClassificationObjectiveFunction(
    std::vector<int> histogram,
    double S_D_Magnitude) {
  double result = 0;
  int totalBodyParts = constants::BODY_PARTS;
  for (int i = 0; i < totalBodyParts; ++i) {
    double P_C_S = (double) histogram[i] / (double) S_D_Magnitude;
    double log_P_C_S = log(P_C_S);
    result += (P_C_S * log_P_C_S);
  }
  return result * -1;
}
  

std::pair<int, int> Communicator::CalculateArgMinValues(Matrix<Cell>& nodeHistograms) {
  double lowestArgMin = 10000000;
  int featuresIndex = 0;
  int thresholdIndex = 0;
  for (int i = 0; i < featuresSize; i++) {
    for (int j = 0; j < thresholdsSize; j++) {
      Cell& currentCell = nodeHistograms[i][j];
      currentCell.argMinValue = GetArgMinValue(currentCell);
      if (currentCell.argMinValue < lowestArgMin) {
        lowestArgMin = currentCell.argMinValue;
        featuresIndex = i;
        thresholdIndex = j;
      }
    }
  }
  std::cout << "lowestArgMin: " << lowestArgMin << std::endl;
  std::cout << "featIndex: " << featuresIndex << "\tthreshIndex: "<< thresholdIndex << std::endl;
  std::cout << "left count: " << nodeHistograms[featuresIndex][thresholdIndex].leftHistogramTotal;
  std::cout << "right count: " << nodeHistograms[featuresIndex][thresholdIndex].rightHistogramTotal;
  return std::make_pair(featuresIndex, thresholdIndex);
}


void Communicator::ExecutePointsSelection(){

}


void Communicator::StartDistributedTraining(){

}

void Communicator::MasterPrint(std::string message){
  if (rank == MPI_MASTER){
    std::cout << message << std::endl;
  }
}

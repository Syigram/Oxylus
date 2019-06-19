#include <iostream>
#include <cmath>


#include <boost/serialization/set.hpp>

#include <oxylus/communication/communicator.h>

#include <oxylus/training/vectors_generator.h>
#include <oxylus/memory/file_reader.h>
#include <oxylus/training/trainer.h>
#include <oxylus/training/tree.h>

using namespace rdf::bpc;


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
  int bestThresholdIndex = 0;
  int bestFeatureIndex = 0;
  int isLeafNode = 0;
  int maxNodesN = pow(2, configObject->GetMaxTreeLevels());
  Trainer trainer(configObject);
  std::vector<NodeVectors> gatheredNodeVectors;
  std::vector<Matrix<Cell>> vecNodeHistograms;
  std::vector<int> leafNodesList;
  std::vector<int> pointsCount;
  for (int treeId = 0; treeId < numOfTrees; treeId++) {
    Tree tree(treeId);
    for (int j = 1; j < maxNodesN; j++) {
      currentNode = j;

      /* check if current node is already in list of leaf nodes */
      if (tree.NodeExistsInLeafNodesList(currentNode, leafNodesList)) {
        leafNodesList = tree.InsertChildrenToLeafNodesList(currentNode, leafNodesList);
        continue;
      }

      /* if not, count the number of points for the given node */
      int pointsPerNode = ImageOperations::GetNumberOfPointsForNode(currentNode, imagesStructureVector);
      mpi::gather(world, pointsPerNode, pointsCount, MPI_MASTER);
      /* gather all counts of points for a given node and check if they have enough
       * points needed for training */
      if (rank == MPI_MASTER) {
        isLeafNode = !NodeHasMinimunPoints(pointsCount);
      }
      mpi::broadcast(world, isLeafNode, MPI_MASTER);
      /* std::cout << "Slave #" << rank << "has isLeafNode" << isLeafNode << std::endl; */

      /* if master determined the node is a leaf node
       * create leaf node and insert it to tree
       * skip training
       * insert its children to the leaf nodes list */
      if (isLeafNode) {
        std::cout << "Slave #" << rank << "inserted leaf node: " << currentNode << std::endl;
        tree.InsertChildrenToLeafNodesList(currentNode, leafNodesList);
        LeafNode* leafNode = trainer.CreateLeafNode(currentNode, imagesStructureVector);
        tree.Insert(leafNode);
        continue;
      }


      NodeVectors nodeVectors = this->BroadcastNodeVectors(currentNode);
      std::cout << "Slave #" << rank << "training node: " << currentNode << std::endl;
      trainer.TrainNode(imagesStructureVector, nodeVectors, treeId);
      mpi::gather(world, nodeVectors, gatheredNodeVectors, MPI_MASTER);

      /* this block reduces the histograms, calculates the argmin value and sends
       * the indexes of the best feature and threshold back to the slave nodes
       * so that they can do the processing of images using these best features found */
      if (rank == MPI_MASTER) {
        CheckValidNodeVectors(gatheredNodeVectors);
        Matrix<Cell> reducedHistograms = ReduceHistograms(gatheredNodeVectors);
        UpdateHistogramsCount(reducedHistograms);
        std::pair<int, int> bestFeatureAndThreshold = FindLowestArgMin(reducedHistograms);
        bestFeatureIndex = std::get<0>(bestFeatureAndThreshold);
        bestThresholdIndex = std::get<1>(bestFeatureAndThreshold);
        std::cout << "best feat from master"  << bestFeatureIndex<< std::endl;
        std::cout << "best thresh from master"  << bestThresholdIndex << std::endl;
        /* Cell& bestCell = reducedHistograms[bestFeatureIndex][bestThresholdIndex]; */
        /* WeakLearnerNode* weakNode = trainer.CreateTrainedNode(currentNode, */
        /*                 bestFeatureIndex, bestThresholdIndex, nodeVectors); */
        /* tree.Insert(weakNode); */
        /* trainer.CheckForLeafNodes(currentNode, bestCell, tree, leafNodesList); */
        /* mpi::broadcast(world, leafNodesList, MPI_MASTER); */
      }
      mpi::broadcast(world, bestThresholdIndex, MPI_MASTER);
      mpi::broadcast(world, bestFeatureIndex, MPI_MASTER);
      WeakLearnerNode* weakLearnerNode = trainer.CreateTrainedNode(currentNode,
                        bestFeatureIndex, bestThresholdIndex, nodeVectors);
      tree.Insert(weakLearnerNode);
      trainer.EvaluateImages(imagesStructureVector, weakLearnerNode);
      delete nodeVectors.featuresVec;
      delete nodeVectors.thresholdsVec;
    }
    ImageOperations::ResetPoints(imagesStructureVector);

  }
}

bool Communicator::CheckValidNodeVectors(std::vector<NodeVectors> &gatheredNodeVectors) {
    for (int i = 0; i < featuresSize; i++) {
      Features f0 = gatheredNodeVectors[0].featuresVec->at(i);
      Features f1 = gatheredNodeVectors[1].featuresVec->at(i);
      Features f2 = gatheredNodeVectors[2].featuresVec->at(i);
      Features f3 = gatheredNodeVectors[3].featuresVec->at(i);
      if (f0 != f1 || f0 != f2 || f0 != f3) {
        std::cout << "found different feature" << std::endl;
        return false;
      }
    }

    for (int i = 0; i < thresholdsSize; i++) {
      int t0 = gatheredNodeVectors[0].thresholdsVec->at(i);
      int t1 = gatheredNodeVectors[1].thresholdsVec->at(i);
      int t2 = gatheredNodeVectors[2].thresholdsVec->at(i);
      int t3 = gatheredNodeVectors[3].thresholdsVec->at(i);
      if (t0 != t1 || t0 != t2 || t0 != t3) {
        std::cout << "found different threshold" << std::endl;
        return false;
      }
    }

    std::cout << "all thresholds and features are equally equal" << std::endl;
    return true;
}


int Communicator::NodeHasMinimunPoints(std::vector<int> pointsCount) {
  int minPointsPerNode = this->configObject->GetStopCondition();
  for (auto count: pointsCount) {
    if (count < minPointsPerNode) {
      return 0;
    }
  }
  return 1;
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
  int newBatchLimit = 1;
  int i = 0;
  for(auto &memoryMessage: this->memoryMessageVec){
    /* int batchSize = memoryMessage.GetBatchSize(); */ /* TODO: use real batch size calc */
    int batchSize = 2;
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


/* void Communicator::CountHistograms( */
/*     Matrix<Cell>& nodeHistograms, */
/*     int bestFeatureIndex, */
/*     int bestThresholdIndex, */
/*     Tree& tree) { */

/*   int leftCount = nodeHistograms[bestFeatureIndex][bestThresholdIndex].leftHistogramTotal; */
/*   int rightCount = nodeHistograms[bestFeatureIndex][bestThresholdIndex].rightHistogramTotal; */
/*   if (!HasMinimunPoints(leftCount)) { */
            
/*   } */
/* } */



double Communicator::GetArgMinValue(Cell& cell) {
  double S_L_Magnitude = (double) cell.leftHistogramTotal;
  double S_R_Magnitude = (double) cell.rightHistogramTotal;
  double S_Magnitude = (double) cell.totalCount;
  double I_L = ClassificationObjectiveFunction(cell.leftHistogram, S_L_Magnitude);
  double I_R = ClassificationObjectiveFunction(cell.rightHistogram, S_R_Magnitude);
  /* std::cout << "s_l=" << S_L_Magnitude << "\ts_r=" << S_R_Magnitude << "\ts_m=" << S_Magnitude << std::endl; */
  /* std::cout << "IL=" << I_L << "\tIR="<< I_R << std::endl; */
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
    if (P_C_S > 0.0) {
      double log_P_C_S = log(P_C_S);
      result += (P_C_S * log_P_C_S);
    }
  }
  return result * -1;
}
  

std::pair<int, int> Communicator::FindLowestArgMin(Matrix<Cell>& nodeHistograms) {
  double lowestArgMin = 10000000;
  int featuresIndex = 0;
  int thresholdIndex = 0;
  for (int i = 0; i < featuresSize; i++) {
    for (int j = 0; j < thresholdsSize; j++) {
      Cell& currentCell = nodeHistograms[i][j];
      double argMinVal = GetArgMinValue(currentCell);
      if (argMinVal < lowestArgMin) {
        lowestArgMin = argMinVal;
        currentCell.argMinValue = argMinVal;
        featuresIndex = i;
        thresholdIndex = j;
      } else {
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

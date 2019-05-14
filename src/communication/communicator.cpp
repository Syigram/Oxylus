#include <iostream>

#include <oxylus/communication/communicator.h>

#include <oxylus/training/vectors_generator.h>
#include <oxylus/memory/file_reader.h>


using namespace rdf::bpc;


Communicator::Communicator(){
  this->rank = world.rank();
  this->size = world.size();
}

Communicator::Communicator(ConfigurationObject* configObject){
  this->rank = world.rank();
  this->size = world.size();
  this->configObject = configObject;
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
  int currentNode = 0;
  int minPointsPerNode = 2000;
  for (int i = 0; i < numOfTrees; i++) {
    int pointsPerNode = ImageOperations::GetNumberOfPointsForNode(currentNode, imagesStructureVector);
    if (pointsPerNode < minPointsPerNode){
      std::cout << "Not enough points for node: " << currentNode << std::endl;
    }
    NodeVectors nodeVectors = this->BroadcastNodeVectors(currentNode);
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
    int batchSize = 20;
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


void Communicator::ExecutePointsSelection(){

}


void Communicator::StartDistributedTraining(){

}

void Communicator::MasterPrint(std::string message){
  if (rank == MPI_MASTER){
    std::cout << message << std::endl;
  }
}

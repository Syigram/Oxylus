#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>

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
  /* this->MasterPrint("gathering memory messages.."); */
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
  std::vector<int> unusedNodesList;
  std::vector<int> pointsCount;
  for (int treeId = 0; treeId < numOfTrees; treeId++) {
    Tree tree(treeId);
    for (int j = 1; j < maxNodesN; j++) {
      currentNode = j;

      /* check if current node is already in list of leaf nodes */
      if (tree.NodeExistsInLeafNodesList(currentNode, unusedNodesList)) {
        unusedNodesList = tree.InsertChildrenToLeafNodesList(currentNode, unusedNodesList);
        continue;
      }

      /* /1* if not, count the number of points for the given node *1/ */
      /* int pointsPerNode = ImageOperations::GetNumberOfPointsForNode(currentNode, imagesStructureVector); */
      /* mpi::gather(world, pointsPerNode, pointsCount, MPI_MASTER); */
      /* /1* gather all counts of points for a given node and check if they have enough */
      /*  * points needed for training *1/ */
      /* if (rank == MPI_MASTER) { */
      /*   isLeafNode = !trainer.NodeHasMinimunPoints(pointsCount); */
      /* } */
      /* mpi::broadcast(world, isLeafNode, MPI_MASTER); */
      /* /1* std::cout << "Slave #" << rank << "has isLeafNode" << isLeafNode << std::endl; *1/ */

      /* /1* if master determined the node is a leaf node */
      /*  * create leaf node and insert it to tree */
      /*  * skip training */
      /*  * insert its children to the leaf nodes list *1/ */
      /* if (isLeafNode) { */
      /*   tree.InsertChildrenToLeafNodesList(currentNode, unusedNodesList); */
      /*   LeafNode* leafNode = trainer.CreateLeafNode(currentNode, imagesStructureVector); */
      /*   tree.Insert(leafNode); */
      /*   continue; */
      /* } */


      NodeVectors nodeVectors = this->BroadcastNodeVectors(currentNode);
      std::cout << "Slave #" << rank << "training node: " << currentNode << std::endl;
      trainer.TrainNode(imagesStructureVector, nodeVectors, treeId);
      mpi::gather(world, nodeVectors, gatheredNodeVectors, MPI_MASTER);

      /* this block reduces the histograms, calculates the argmin value and sends
       * the indexes of the best feature and threshold back to the slave nodes
       * so that they can do the processing of images using these best features found */
      if (rank == MPI_MASTER) {
        /* CheckValidNodeVectors(gatheredNodeVectors); */
        Matrix<Cell> reducedHistograms = trainer.ReduceHistograms(gatheredNodeVectors);
        trainer.UpdateHistogramsCount(reducedHistograms);
        std::pair<int, int> bestFeatureAndThreshold = trainer.FindLowestArgMin(reducedHistograms);
        bestFeatureIndex = std::get<0>(bestFeatureAndThreshold);
        bestThresholdIndex = std::get<1>(bestFeatureAndThreshold);
        Cell& bestCell = reducedHistograms[bestFeatureIndex][bestThresholdIndex];
        WeakLearnerNode* weakNode = trainer.CreateTrainedNode(currentNode,
                        bestFeatureIndex, bestThresholdIndex, nodeVectors);
        tree.Insert(weakNode);
        trainer.CheckForLeafNodes(currentNode, bestCell, tree, unusedNodesList);
      }
      mpi::broadcast(world, unusedNodesList, MPI_MASTER);
      mpi::broadcast(world, bestThresholdIndex, MPI_MASTER);
      mpi::broadcast(world, bestFeatureIndex, MPI_MASTER);
      WeakLearnerNode* weakLearnerNode = trainer.CreateTrainedNode(currentNode,
                        bestFeatureIndex, bestThresholdIndex, nodeVectors);
      trainer.EvaluateImages(imagesStructureVector, weakLearnerNode);
      delete nodeVectors.featuresVec;
      delete nodeVectors.thresholdsVec;
    }
    ImageOperations::ResetPoints(imagesStructureVector);
    
    if (rank == MPI_MASTER) {
      tree.Serialize();
      tree.EraseTree();
    }

  }
}

/* TODO: Fix this function. Currently only works for 4 process. If there are
 * more or less processes it will break. */
void Communicator::CheckValidNodeVectors(std::vector<NodeVectors> &gatheredNodeVectors) {
    for (int i = 0; i < featuresSize; i++) {
      Features f0 = gatheredNodeVectors[0].featuresVec->at(i);
      Features f1 = gatheredNodeVectors[1].featuresVec->at(i);
      Features f2 = gatheredNodeVectors[2].featuresVec->at(i);
      Features f3 = gatheredNodeVectors[3].featuresVec->at(i);
      if (f0 != f1 || f0 != f2 || f0 != f3) {
        std::cerr << "Error: Features objects aren't equal across all computing nodes" << std::endl;
        std::exit(EXIT_FAILURE);
      }
    }

    for (int i = 0; i < thresholdsSize; i++) {
      int t0 = gatheredNodeVectors[0].thresholdsVec->at(i);
      int t1 = gatheredNodeVectors[1].thresholdsVec->at(i);
      int t2 = gatheredNodeVectors[2].thresholdsVec->at(i);
      int t3 = gatheredNodeVectors[3].thresholdsVec->at(i);
      if (t0 != t1 || t0 != t2 || t0 != t3) {
        std::cerr << "Error: Threshold objects aren't equal across all computing nodes" << std::endl;
        std::exit(EXIT_FAILURE);
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
    int batchSize = 1;
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

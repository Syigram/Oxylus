#include <oxylus/training/weak_learner_node.h>


using namespace rdf::bpc;

WeakLearnerNode::WeakLearnerNode() {
  this->rightNode = nullptr;
  this->leftNode = nullptr;
}

WeakLearnerNode::WeakLearnerNode(int id) {
  this->rightNode = nullptr;
  this->leftNode = nullptr;
  this->nodeId = id;
}

int WeakLearnerNode::GetNodeId() {
  return nodeId;
}

void WeakLearnerNode::Print() {
  std::cout << "LearningNode: " << nodeId << "\tTreshold: " << threshold << std::endl;
  features.Print();
}


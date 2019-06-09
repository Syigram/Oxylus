#include <oxylus/training/weak_learner_node.h>


using namespace rdf::bpc;

WeakLearnerNode::WeakLearnerNode() {
  this->rightNode = NULL;
  this->leftNode = NULL;
}

WeakLearnerNode::WeakLearnerNode(int id) {
  this->rightNode = NULL;
  this->leftNode = NULL;
  this->nodeId = id;
}

int WeakLearnerNode::GetNodeId() {
  return nodeId;
}


#include <oxylus/training/leaf_node.h>
#include <oxylus/configuration/configuration_constants.h>


using namespace rdf::bpc;


LeafNode::LeafNode(){
  this->histogram.resize(rdf::bpc::constants::BODY_PARTS, 0);
  this->rightNode = nullptr;
  this->leftNode = nullptr;
}


LeafNode::LeafNode(int nodeId) {
  this->nodeId = nodeId;
  this->histogram.resize(rdf::bpc::constants::BODY_PARTS, 0);
  this->rightNode = nullptr;
  this->leftNode = nullptr;
}

LeafNode::LeafNode(int nodeId, std::vector<int> histogram) {
  this->nodeId = nodeId;
  this->histogram = histogram;
  this->rightNode = nullptr;
  this->leftNode = nullptr;
}

int LeafNode::GetNodeId() {
  return nodeId;
}


void LeafNode::Print() {
  std::cout << "LeafNode: " << nodeId << std::endl;
}

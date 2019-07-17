#include <fstream>
#include <algorithm>
#include <string>
#include <sys/stat.h>


#include <oxylus/training/tree.h>
#include <boost/filesystem.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>

using namespace rdf::bpc;

Tree::Tree(){
  root = NULL;
}

Tree::Tree(std::string name){
  root = NULL;
  this->name = name;
}

Tree::Tree(int id){
  root = NULL;
  this->id = id;
  this->name = std::to_string(id);
}

void Tree::EraseTree() {
  EraseTree(root);
}

void Tree::EraseTree(Node* node) {
  if (node != nullptr) {
    EraseTree(node->GetLeft());
    EraseTree(node->GetRight());
    delete node;
  }
}

std::vector<int> Tree::InsertChildrenToLeafNodesList(int nodeId, std::vector<int>& leafNodes) {
  int leftNodeId = nodeId * 2;
  int rightNodeId = (nodeId * 2) + 1;
  leafNodes.push_back(leftNodeId);
  leafNodes.push_back(rightNodeId);
  return leafNodes;
}

bool Tree::NodeExistsInLeafNodesList(int nodeId, std::vector<int>& leafNodes) {
  auto it = std::find(leafNodes.begin(), leafNodes.end(), nodeId);
  if (it != leafNodes.end()) {
    leafNodes.erase(it);
    return true;
  } else {
    return false;
  }

}


void Tree::Insert(Node* node){
  if (root == NULL) {
    root = node;
  } else {
    int nodeId = node->GetNodeId();
    std::vector<int> parentsList = GetParentsList(nodeId);
    Node* currentNode = GetRoot();
    for (auto parentId: parentsList) {
      currentNode = GetNextNode(currentNode, parentId);
    }
    InsertAfterNode(currentNode, node);
  }
  /* return node; */
}

void Tree::InsertAfterNode(Node* parentNode, Node* newNode) {
  int newNodeId = newNode->GetNodeId();
  int parentId = parentNode->GetNodeId();
  if (IsRightChild(parentId, newNodeId)) {
    parentNode->SetRightNode(newNode);
  } else {
    parentNode->SetLeftNode(newNode);
  }
}


Node* Tree::GetNextNode(Node* prevNode, int nextNodeId) {
  int prevId = prevNode->GetNodeId();
  if (IsRightChild(prevId, nextNodeId)) {
    return prevNode->GetRight();
  } else {
    return  prevNode->GetLeft();
  }

}


bool Tree::IsLeftChild(int prevNodeId, int nextNodeId) {
  return !IsRightChild(prevNodeId, nextNodeId);
}


bool Tree::IsRightChild(int prevNodeId, int nextNodeId) {
  if (nextNodeId == (prevNodeId * 2) + 1) {
    return true;
  }
  return false;
}


void Tree::SaveNode(Node* node) {
  std::ofstream outputFile;
  std::string filepath = "tree.txt";
  outputFile.open(filepath, std::ios::out | std::ios::in );
  outputFile.close();
}


void Tree::SaveTree(Node* node) {
  if (node != NULL) {
    SaveTree(node->GetLeft());
    SaveNode(node);
    SaveTree(node->GetRight());
  }
}


void Tree::PrintNode(Node* node) {
  node->Print();
}


void Tree::PrintTree(Node* node) {
  if (node != NULL) {
    PrintTree(node->GetLeft());
    PrintNode(node);
    PrintTree(node->GetRight());
  }
  
}


void Tree::Serialize(ConfigurationObject* configObject) {
  std::string folder = configObject->GetOutputFolder();
  char folderC[folder.size() + 1];
  folder.copy(folderC, folder.size() + 1);
  folderC[folder.size()] = '\0';
  std::string filename = folder + "/tree" + this->name + ".dat";
  if (!boost::filesystem::exists(folder)) {
    mkdir(folderC, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  }
  std::ofstream ofs(filename);
  std::cout << "Serializing tree to file: " << filename << std::endl;
  {
      boost::archive::text_oarchive oa(ofs);
      oa.register_type<WeakLearnerNode>();
      oa.register_type<LeafNode>();
      oa << *this;
  }
}


Tree Tree::Deserialize(std::string filename) {
  std::cout << "Deserializing tree from file: " << filename << std::endl;
  {
      std::ifstream ifs(filename);
      boost::archive::text_iarchive ia(ifs);
      ia.register_type<WeakLearnerNode>();
      ia.register_type<LeafNode>();
      ia >> *this;
  }
  return *this;
}


void Tree::SetRoot(Node* node) {
  this->root = node;
}


Node* Tree::GetRoot() {
  return this->root;
}

std::vector<int> Tree::GetParentsList(int nodeId) {
  std::vector<int> parentsList;
  while ((nodeId >>= 1) > 0) {
    parentsList.push_back(nodeId);
  }
  parentsList.pop_back(); /* removes root node from list */
  std::reverse(parentsList.begin(), parentsList.end());
  return parentsList;
}

int Tree::GetParentId(int nodeId) {
  return nodeId >> 1;
}

int Tree::GetNodeLevel(int nodeId) {
  int count = 1;
  while ( (nodeId >>= 1) > 0) count++;
  return count;
}


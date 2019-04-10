#include <iostream>

#include <oxylus/image/point_structure.h>


using namespace rdf::bpc;


PointStructure::PointStructure() {
  this->currentNode = 0;
  this->x = 0;
  this->y = 0;
  this->imageId = 0;
  this->depthPixelValue = 0;
  this->labelPixelValue = 0;
}

PointStructure::PointStructure(int node, int depthValue, int labelValue,
                                  int x, int y, int imageId) {
  this->currentNode = node;
  this->x = x;
  this->y = y;
  this->imageId = imageId;
  this->depthPixelValue = depthValue;
  this->labelPixelValue = labelValue;
}

void PointStructure::SetCurrentNode(int currentNode) {
  this->currentNode = currentNode;
}
int PointStructure::GetCurrentNode() {
  return this->currentNode;
}
void PointStructure::SetDepthPixelValue(int depthPixelValue) {
  this->depthPixelValue = depthPixelValue;
}
int PointStructure::GetDepthPixelValue() {
  return this->depthPixelValue;
}
void PointStructure::SetLabelPixelValue(int labelPixelValue) {
  this->labelPixelValue = labelPixelValue;
}
int PointStructure::GetLabelPixelValue() {
  return this->labelPixelValue;
}
void PointStructure::SetX(int x) {
  this->x = x;
}
int PointStructure::GetX() {
  return this->x;
}
void PointStructure::SetY(int y) {
  this->y = y;
}
int PointStructure::GetY() {
  return this->y;
}
void PointStructure::SetImageId(int imageId) {
  this->imageId = imageId;
}
int PointStructure::GetImageId() {
  return this->imageId;
}

void PointStructure::Print() {
  std::cout << "Node: " << currentNode << "\t";
  std::cout << "Depth: " << depthPixelValue << "\t";
  std::cout << "Label: " << labelPixelValue << "\t";
  std::cout << "X: " << x << "\t";
  std::cout << "Y: " << y << "\t";
  std::cout << "ImageId: " << imageId << std::endl;

}

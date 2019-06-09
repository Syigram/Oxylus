#include <iostream>

#include <oxylus/image/point_structure.h>


using namespace rdf::bpc;


PointStructure::PointStructure() {
  this->currentNode = 1;
  this->point.x = 0;
  this->point.y = 0;
  this->imageId = 0;
  this->labelPixelValue = 0;
}

PointStructure::PointStructure(int labelValue,
                                  int x, int y, int imageId) {
  this->currentNode = 1;
  this->point.x = x;
  this->point.y = y;
  this->imageId = imageId;
  this->labelPixelValue = labelValue;
}

PointStructure::PointStructure(cv::Point point, int labelValue, int imageId) {
  this->currentNode = 1;
  this->point = point;
  this->imageId = imageId;
  this->labelPixelValue = labelValue;
}

void PointStructure::SetCurrentNode(int currentNode) {
  this->currentNode = currentNode;
}
int PointStructure::GetCurrentNode() {
  return this->currentNode;
}
void PointStructure::SetLabelPixelValue(int labelPixelValue) {
  this->labelPixelValue = labelPixelValue;
}
int PointStructure::GetLabelPixelValue() {
  return this->labelPixelValue;
}
void PointStructure::SetX(int x) {
  this->point.x = x;
}
int PointStructure::GetX() {
  return this->point.x;
}
void PointStructure::SetY(int y) {
  this->point.y = y;
}
int PointStructure::GetY() {
  return this->point.y;
}

void PointStructure::SetPoint(int x, int y){
  this->point.x = x;
  this->point.y = y;
}

cv::Point PointStructure::GetPoint(){
  return this->point;
}

void PointStructure::SetImageId(int imageId) {
  this->imageId = imageId;
}
int PointStructure::GetImageId() {
  return this->imageId;
}

void PointStructure::Print() {
  std::cout << "Node: " << currentNode << "\t";
  std::cout << "Label: " << labelPixelValue << "\t";
  std::cout << "X: " << point.x << "\t";
  std::cout << "Y: " << point.y << "\t";
  std::cout << "ImageId: " << imageId << std::endl;

}

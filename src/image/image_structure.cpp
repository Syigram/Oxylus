#include <oxylus/image/image_structure.h>


using namespace rdf::bpc;

ImageStructure::ImageStructure() {

}

ImageStructure::ImageStructure(ConfigurationObject* configObject) {
  this->rows = configObject->GetImagesRows();
  this->cols = configObject->GetImagesCols();
  this->pointsSize = configObject->GetPointsSize();
  this->treesSize = configObject->GetNumberOfTrees();
  treesId.resize(treesSize, 0);
}

ImageStructure::ImageStructure(ConfigurationObject* configObject, int imageId) {
  this->rows = configObject->GetImagesRows();
  this->cols = configObject->GetImagesCols();
  this->pointsSize = configObject->GetPointsSize();
  this->treesSize = configObject->GetNumberOfTrees();
  this->imageId = imageId;
  treesId.resize(treesSize, 0);
}

ImageStructure::ImageStructure(ConfigurationObject* configObject, cv::Mat_<ushort> depthImage, int imageId) {
  this->rows = configObject->GetImagesRows();
  this->cols = configObject->GetImagesCols();
  this->pointsSize = configObject->GetPointsSize();
  this->treesSize = configObject->GetNumberOfTrees();
  this->imageId = imageId;
  this->depthImage = depthImage;
  treesId.resize(treesSize, 0);
}

int ImageStructure::GetSizeOf(ConfigurationObject* configObject) {
  int totalPoints = configObject->GetPointsSize();
  int totalTrees = configObject->GetNumberOfTrees();
  int pointsStructuresBytes = totalPoints * sizeof(PointStructure);
  int pointPositionBytes = totalPoints * sizeof(std::pair<int, int>);
  int integerDataMembersBytes = sizeof(int) * 5;
  int treesIdsBytes = totalTrees * sizeof(int);
  int depthImageSize = this->depthImage.total() * this->depthImage.elemSize();
  int pointerPointsVec = sizeof(this->pointsVector);
  return (pointPositionBytes + pointsStructuresBytes + treesIdsBytes +
          + pointerPointsVec + integerDataMembersBytes + depthImageSize) / KILOBYTE;
}


int ImageStructure::GetPointsSize() {
  return this->pointsSize;
}

int ImageStructure::GetRows() {
  return this->rows;
}


int ImageStructure::GetImageId() {
  return this->imageId;
}

int ImageStructure::GetCols() {
  return this->cols;
}

std::shared_ptr<PointStructureVec> ImageStructure::GetPointsVector() {
  return this->pointsVector;
}

void ImageStructure::SetPointsVector(std::shared_ptr<PointStructureVec> pointsVector) {
  this->pointsVector = pointsVector;
}

void ImageStructure::SetDepthImage(cv::Mat_<ushort> depthImage){
  this->depthImage = depthImage;
}

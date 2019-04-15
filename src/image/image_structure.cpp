#include <oxylus/image/image_structure.h>


using namespace rdf::bpc;

ImageStructure::ImageStructure() {

}

ImageStructure::ImageStructure(ConfigurationObject* configObject) {
  this->rows = configObject->GetImagesRows();
  this->cols = configObject->GetImagesCols();
  this->pointsSize = configObject->GetPointsSize();
  this->treesSize = configObject->GetNumberOfTrees();
}

ImageStructure::ImageStructure(ConfigurationObject* configObject, int imageId) {
  this->rows = configObject->GetImagesRows();
  this->cols = configObject->GetImagesCols();
  this->pointsSize = configObject->GetPointsSize();
  this->imageId = imageId;
}

int ImageStructure::GetSizeOf(ConfigurationObject* configObject) {
  int totalPoints = configObject->GetPointsSize();
  int totalTrees = configObject->GetNumberOfTrees();
  int pointsStructuresBytes = totalPoints * sizeof(PointStructure);
  int pointPositionBytes = totalPoints * sizeof(std::pair<int, int>);
  int integerDataMembersBytes = sizeof(int) * 5;
  int treesIdsBytes = totalTrees * sizeof(int);
  return (pointPositionBytes + pointsStructuresBytes + treesIdsBytes + integerDataMembersBytes) / KILOBYTE;
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

std::shared_ptr<PointsVector> ImageStructure::GetPointsVector() {
  return this->pointsVector;
}

void ImageStructure::SetPointsVector(std::shared_ptr<PointsVector> pointsVector) {
  this->pointsVector = pointsVector;
}

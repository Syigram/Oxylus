#include <oxylus/image/image_structure.h>


using namespace rdf::bpc;

ImageStructure::ImageStructure() {

}

ImageStructure::ImageStructure(ConfigurationObject* configObject) {
  this->rows = configObject->GetImagesRows();
  this->cols = configObject->GetImagesCols();
  this->pointsSize = configObject->GetImagesNumber();
}

ImageStructure::ImageStructure(ConfigurationObject* configObject, int imageId) {
  this->rows = configObject->GetImagesRows();
  this->cols = configObject->GetImagesCols();
  this->pointsSize = configObject->GetImagesNumber();
  this->imageId = imageId;
}

int ImageStructure::GetSizeOf() {
  int labelSize = labelImage.total() * labelImage.elemSize();
  int depthSize = depthImage.total() * depthImage.elemSize();
  int treesVecSize = sizeof(treesId);
  int pointsSize = sizeof(setOfPoints);
  int rowsSize = sizeof(int);
  int colsSize = sizeof(int);
  return (labelSize + depthSize + treesVecSize + pointsSize + rowsSize) / KILOBYTE;

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

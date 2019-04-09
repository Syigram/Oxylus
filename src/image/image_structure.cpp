#include <oxylus/image/image_structure.h>


using namespace rdf;

ImageStructure::ImageStructure() {
  int n = 1000;
  this->rows = n;
  this->cols = n;
  labelImage = cv::Mat_<uchar>(rows, cols);
  depthImage = cv::Mat_<ushort>(rows, cols);
}


ImageStructure::ImageStructure(ConfigurationObject* configObject) {
  this->rows = configObject->GetImagesRows();
  this->cols = configObject->GetImagesCols();
  labelImage = cv::Mat_<uchar>(rows, cols);
  depthImage = cv::Mat_<ushort>(rows, cols);
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

int ImageStructure::GetRows() {
  return this->rows;
}


int ImageStructure::GetCols() {
  return this->cols;
}

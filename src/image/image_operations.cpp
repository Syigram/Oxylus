#include <iostream>
#include <fstream>

#include <oxylus/configuration/configuration_constants.h>
#include <oxylus/image/image_operations.h>
#include <oxylus/memory/helper.h>

using namespace rdf::bpc;

ImageOperations::ImageOperations(){

}

MapPalette ImageOperations::InitializePalette() {
  MapPalette palette;
  palette[cv::Vec3b(0,0,0)] = rdf::bpc::constants::BLACK;
  palette[cv::Vec3b(0,0,255)] = rdf::bpc::constants::BLUE;
  palette[cv::Vec3b(0,255,0)] = rdf::bpc::constants::GREEN;
  palette[cv::Vec3b(0,255,255)] = rdf::bpc::constants::CIAN;
  palette[cv::Vec3b(255,0,0)] = rdf::bpc::constants::RED;
  palette[cv::Vec3b(255,0,255)] = rdf::bpc::constants::MAGENTA;
  palette[cv::Vec3b(255,255,0)] = rdf::bpc::constants::YELLOW;
  palette[cv::Vec3b(255,255,255)] = rdf::bpc::constants::WHITE;
  return palette;
}


void ImageOperations::PrintPalette(MapPalette palette){
  std::cout << "La paleta:\n ";
  for (auto& pe:palette) {
    std::cout <<"\t("
      << int((pe.first)[0]) << ","
      << int((pe.first)[1]) << ","
      << int((pe.first)[2]) << ") -> "
      << int(pe.second) << std::endl;
  }
}

void ImageOperations::SavePaletteTo(std::string filepath, MapPalette& palette){
  std::ofstream outputFile;
  outputFile.open(filepath, std::ios::out | std::ios::in );
  for (auto& pe:palette) {
    outputFile <<"("
      << int((pe.first)[0]) << ","
      << int((pe.first)[1]) << ","
      << int((pe.first)[2]) << ") -> "
      << int(pe.second) << std::endl;
  }
  outputFile.close();
}

std::shared_ptr<PointsVector> ImageOperations::GenerateRandomPoints(ImageStructure& imageData,
                                                   cv::Mat_<ushort> depthImage,
                                                   cv::Mat_<uchar> labelImage) {
  std::pair<std::set<std::pair<int,int>>::iterator,bool> ret;
  std::uniform_int_distribution<int> rows_dist(0, imageData.GetRows() - 1);
  std::uniform_int_distribution<int> cols_dist(0, imageData.GetCols() - 1);
  int numberOfPoints = imageData.GetPointsSize();
  int imageId = imageData.GetImageId();
  int counter = 0;
  int nodeId = 0;
  std::set<std::pair<int,int>> setOfPoints;
  std::shared_ptr<PointsVector> pointsVector = std::make_shared<PointsVector>();
  while (counter < numberOfPoints){
    int x = rows_dist(mt_);
    int y = cols_dist(mt_);
    std::pair<int, int> coords = std::make_pair(x, y);
    ret = setOfPoints.insert(coords);
    if (ret.second == true) {/* insert is successful */
      int pixelValueLabel = (int) labelImage.at<uchar>(x, y);
      int pixelValueDepth = (int) depthImage.at<ushort>(x, y);
      PointStructure point(nodeId, pixelValueDepth, pixelValueLabel, x, y, imageId);
      pointsVector->push_back(point);
      point.Print();
      std::cout << "couter" << counter;
      counter++;
    }
  }
  return pointsVector;
}

std::set<std::pair<int,int>> ImageOperations::RandomPointsSelection(int rows, int cols, int numberOfPoints) {
  std::pair<std::set<std::pair<int,int>>::iterator,bool> ret;
  std::uniform_int_distribution<int> rows_dist(0, rows - 1);
  std::uniform_int_distribution<int> cols_dist(0, cols - 1);
  int counter = 0;
  std::set<std::pair<int,int>> setOfPoints;
  while (counter < numberOfPoints){
    int randomRow = rows_dist(mt_);
    int randomCol = cols_dist(mt_);
    std::pair<int, int> coords = std::make_pair(randomRow, randomCol);
    ret = setOfPoints.insert(coords);
    if (ret.second == true) {/* insert is successful */
      counter++;
    }
  }
  std::cout << "size of set" << setOfPoints.size() << std::endl;
  /* Helper::PrintIterable(setOfPoints); */
  for (auto& elem : setOfPoints) {
    std::cout << elem.first << "\t" << elem.second << std::endl;
  }
  return setOfPoints;
}


std::mt19937 ImageOperations::InitRandomSeed() {
    std::random_device rd;
    std::mt19937 mt(rd());
    return mt;
}

std::mt19937 ImageOperations::mt_ = ImageOperations::InitRandomSeed();

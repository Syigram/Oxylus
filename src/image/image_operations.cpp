#include <iostream>
#include <fstream>

#include <oxylus/configuration/configuration_constants.h>
#include <oxylus/image/image_operations.h>
#include <oxylus/memory/helper.h>

using namespace rdf::bpc;


ImageOperations::ImageOperations(){

}

ImageOperations::ImageOperations(ConfigurationObject* configurationObject){
  this->treesPercent = configurationObject->GetPercentImagesPerTree();
  this->numOfTrees = configurationObject->GetNumberOfTrees();
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

int ImageOperations::GetNumberOfPointsForNode(int nodeId, std::shared_ptr<ImagesVector> imagesVector){
  int counter = 0;
  for (auto& image: *imagesVector) {
    for (auto& point: *(image.pointsVector)) {
      if (point.GetCurrentNode() == nodeId){
        counter++;
      }
    }
  }
  return counter;

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

std::shared_ptr<PointStructureVec> ImageOperations::GenerateRandomPoints(ImageStructure& imageData,
                                                   cv::Mat_<ushort>& depthImage,
                                                   cv::Mat_<uchar>& labelImage,
                                                   MapPalette palette) {
  std::pair<std::set<cv::Point>::iterator,bool> ret;
  int rows = imageData.GetRows();
  int cols = imageData.GetCols();
  int totalPoints = imageData.GetPointsSize();
  int imageId = imageData.GetImageId();
  int initialNodeId = 0;
  std::set<cv::Point> setOfPoints;
  std::vector<cv::Point> vectorOfPoints;
  std::shared_ptr<PointStructureVec> pointsStructVec = std::make_shared<PointStructureVec>();
  VecColorsHistogram vecColorHistogram = ImageOperations::GenerateColorsHistogramForImage(labelImage, rows, cols);
  int totalColorsInImage = vecColorHistogram.size();
  int newLimit = totalPoints / totalColorsInImage;
  int newCounter = 0;
  int index = 0;
  for (auto& colorPoints: vecColorHistogram){
    int totalColorPoints = colorPoints.second.size();
    index++;
    while (newCounter <= newLimit){
      if (newCounter == totalColorPoints){
        /* if points inserted are already at max for a color
         * we calculate the remaining points, then we calculate
         * the new limit according to those points and accounting
         * for the colors already used, with the index variable. */
        totalPoints = totalPoints - totalColorPoints;
        newLimit = totalPoints / (totalColorsInImage - index);
        break;
      }
      std::uniform_int_distribution<int> index_dist(0, totalColorPoints - 1);
      int randomIndex = index_dist(ImageOperations::mt_);
      auto point = colorPoints.second.at(randomIndex);
      if (!ImageOperations::PointExistsInVector(vectorOfPoints, point)){
        int labelValue = (int) labelImage.at<uchar>(point);
        PointStructure pointStructure(point, labelValue, imageId, initialNodeId);
        pointsStructVec->push_back(pointStructure);
        vectorOfPoints.push_back(point);
        newCounter++;
      }
    }
    newCounter = 0;
  }
  return pointsStructVec;
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

bool ImageOperations::PointExistsInVector(std::vector<cv::Point> pointsVector, cv::Point point){
  for (auto& currentPoint: pointsVector){
    int currentX = currentPoint.x;
    int currentY = currentPoint.y;
    if (currentX == point.x && currentY == point.y){
      return true;
    }
  }
  return false;
}

VecColorsHistogram ImageOperations::GenerateColorsHistogramForImage(cv::Mat_<uchar>& labelImage,
                                                                    int rows, int cols){
  MapColorsHistogram mapColorsHistogram;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      /* for each point:
       * save its color and position */
      cv::Point point(j, i);
      int labelValue = (int) labelImage.at<uchar>(i, j);
      mapColorsHistogram[labelValue].push_back(point);
    }
  }
  VecColorsHistogram orderedVector = ImageOperations::SortColorsHistogram(mapColorsHistogram);
  return orderedVector;
}


VecColorsHistogram ImageOperations::SortColorsHistogram(MapColorsHistogram& mapColorsHistogram){
  typedef std::pair<int, std::vector<cv::Point>> PairColorToPoints;
  VecColorsHistogram orderedColorsHistogram;
  int size = mapColorsHistogram.size();
  int highVal = 10000000;
  int index = 0;
  int lowestValue = highVal;
  while (mapColorsHistogram.size()){
    for (auto& elem: mapColorsHistogram){
      int currentLow = elem.second.size();
      if (currentLow < lowestValue && currentLow){
        lowestValue = currentLow;
        index = elem.first;
      }
    }
    PairColorToPoints pairColorPoints = std::make_pair(index, mapColorsHistogram[index]);
    orderedColorsHistogram.push_back(pairColorPoints);
    mapColorsHistogram.erase(index);
    lowestValue = highVal;
  }
  return orderedColorsHistogram;
}

int ImageOperations::AssignImagesToTrees(std::shared_ptr<ImagesVector> imagesVector){
  int totalImages = imagesVector->size();
  int percentPerImages = this->treesPercent;
  float percentValueFloat = (float) (percentPerImages / 100.0);
  percentValueFloat = percentValueFloat * (float) totalImages;
  int percentValueInt = (int) percentValueFloat;
  int imageCount = 0;
  std::uniform_int_distribution<int> index_dist(0, totalImages - 1);
  for (int i = 0; i < this->numOfTrees; i++){
    while (imageCount < percentValueInt) {
      int imageIndex = index_dist(ImageOperations::mt_);
      int treeBit = imagesVector->at(imageIndex).treesId[i];
      if (treeBit == 0){
        imagesVector->at(imageIndex).treesId[i] = 1;
        imageCount++;
      }
    }
    imageCount = 0;
  }
}


std::mt19937 ImageOperations::mt_ = ImageOperations::InitRandomSeed();

#include <iostream>

#include <oxylus/training/vectors_generator.h>

using namespace rdf::bpc;

VectorsGenerator::VectorsGenerator(){

}

VectorsGenerator::VectorsGenerator(ConfigurationObject* configObject){
  this->minThresholdsValue = configObject->GetThresholdsMinValue();
  this->maxThresholdsValue = configObject->GetThresholdsMaxValue();
  this->minFeaturesValue = configObject->GetFeaturesMinValue();
  this->maxFeaturesValue = configObject->GetFeaturesMaxValue();
  this->thresholdsVectorSize = configObject->GetThresholdsSize();
  this->featuresVectorSize = configObject->GetFeaturesSize();
  this->minDeltaXOffset = configObject->GetDeltaMinXOffset();
  this->maxDeltaXOffset = configObject->GetDeltaMaxXOffset();
  this->minDeltaYOffset = configObject->GetDeltaMinYOffset();
  this->maxDeltaYOffset = configObject->GetDeltaMaxYOffset();
}

std::vector<Features>* VectorsGenerator::GenerateFeaturesVector(){
  std::uniform_int_distribution<int> vertical_offset(minDeltaXOffset, maxDeltaXOffset);
  std::uniform_int_distribution<int> horizontal_offset(minDeltaYOffset, maxDeltaYOffset);
  std::vector<Features>* featuresVector = new std::vector<Features>;
  featuresVector->reserve(featuresVectorSize);
  for (size_t i = 0; i < featuresVectorSize; i++){
    Features features;
    int x1 = horizontal_offset(mt_);
    int y1 = vertical_offset(mt_);
    int x2 = horizontal_offset(mt_);
    int y2 = vertical_offset(mt_);
    features.SetDelta1(y1, x1);
    features.SetDelta2(y2, x2);
    featuresVector->push_back(features);
  }
  return featuresVector;
}

std::vector<int>* VectorsGenerator::GenerateThresholdsVector(){
  std::uniform_int_distribution<int> threshold_dist(minThresholdsValue, maxThresholdsValue);
  std::vector<int>* thresholdsVector = new std::vector<int>;
  thresholdsVector->reserve(thresholdsVectorSize);
  for (size_t i = 0; i < thresholdsVectorSize; i++){
    int randomValue = threshold_dist(mt_);
    thresholdsVector->push_back(randomValue);
  }
  return thresholdsVector;
}

std::mt19937 VectorsGenerator::InitializeRandomDevice(){
  std::random_device rd;
  std::mt19937 mt(rd());
  return mt;
}

std::mt19937 VectorsGenerator::mt_ = VectorsGenerator::InitializeRandomDevice();


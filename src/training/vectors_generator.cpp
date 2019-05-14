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
}

std::vector<Features>* VectorsGenerator::GenerateFeaturesVector(){
  std::uniform_int_distribution<int> features_dist(minFeaturesValue, maxFeaturesValue);
  std::vector<Features>* featuresVector = new std::vector<Features>();
  featuresVector->reserve(featuresVectorSize);
  for (size_t i = 0; i < featuresVectorSize; i++){
    Features features;
    int value1 = features_dist(mt_);
    int value2 = features_dist(mt_);
    int value3 = features_dist(mt_);
    int value4 = features_dist(mt_);
    features.SetDelta1(value1, value2);
    features.SetDelta2(value3, value4);
    featuresVector->push_back(features);
  }
  return featuresVector;
}

std::vector<int>* VectorsGenerator::GenerateThresholdsVector(){
  std::uniform_int_distribution<int> threshold_dist(minThresholdsValue, maxThresholdsValue);
  std::vector<int>* thresholdsVector = new std::vector<int>();
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


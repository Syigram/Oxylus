#include <memory>

#include <oxylus/configuration/configuration_object.h>

using namespace rdf;

ConfigurationObject::ConfigurationObject() {
  std::shared_ptr<ConfigurationParser> configParser(new ConfigurationParser());
  this->configParser = configParser;
  LoadConfiguration();
}


ConfigurationObject::ConfigurationObject(std::string filename) {
  std::shared_ptr<ConfigurationParser> configParser(new ConfigurationParser(filename));
  this->configParser = configParser;
  LoadConfiguration();
}

int ConfigurationObject::GetPercentImagesPerTree() {
  return this->percentImagesPerTree;
}

int ConfigurationObject::GetThresholdsSize() {
  return this->thresholdsSize;
}

int ConfigurationObject::GetFeaturesSize() {
  return this->featuresSize;
}

int ConfigurationObject::GetFeaturesMinValue() {
  return this->featuresMinValue;
}

int ConfigurationObject::GetFeaturesMaxValue() {
  return this->featuresMaxValue;
}

int ConfigurationObject::GetThresholdsMinValue() {
  return this->thresholdsMinValue;
}

int ConfigurationObject::GetThresholdsMaxValue() {
  return this->thresholdsMaxValue;
}

int ConfigurationObject::GetImagesCols() {
  return this->imagesCols;
}

int ConfigurationObject::GetImagesRows() {
  return this->imagesRows;
}

int ConfigurationObject::GetImagesNumber() {
  return this->imagesNumber;
}

int ConfigurationObject::GetPointsSize() {
  return this->pointsSize;
}

int ConfigurationObject::GetMemoryUsage() {
  return this->memoryUsage;
}

int ConfigurationObject::GetNumberOfTrees() {
  return this->numberOfTrees;
}

int ConfigurationObject::GetImagesPerNode() {
  return this->imagesPerNode;
}

int ConfigurationObject::GetMaxTreeLevels() {
  return this->maxTreeLevels;
}

std::string ConfigurationObject::GetBaseFolder(){
  return this->baseFolder;
}

std::string ConfigurationObject::GetLabelFolder(){
  return this->labelFolder;
}

std::string ConfigurationObject::GetDepthFolder(){
  return this->depthFolder;
}

std::string ConfigurationObject::GetFileExtension(){
  return this->fileExtension;
}

std::string ConfigurationObject::GetFilePrefix(){
  return this->filePrefix;
}

int ConfigurationObject::LoadConfiguration()  {
  this->imagesCols = configParser->LoadImagesCols();
  this->imagesRows = configParser->LoadImagesRows();
  this->imagesNumber = configParser->LoadImagesNumber();
  this->baseFolder = configParser->LoadBaseFolder();
  this->labelFolder = configParser->LoadLabelFolder();
  this->depthFolder = configParser->LoadDepthFolder();
  this->fileExtension = configParser->LoadFileExtension();
  this->filePrefix = configParser->LoadFilePrefix();
  this->pointsSize = configParser->LoadPointsSize();
  this->memoryUsage = configParser->LoadMemoryUsage();
  this->maxTreeLevels = configParser->LoadMaxTreeLevels();
  this->numberOfTrees = configParser->LoadNumberOfTrees();
  this->imagesPerNode = configParser->LoadImagesPerNode();
  this->featuresMinValue = configParser->LoadFeaturesMinValue();
  this->featuresMaxValue = configParser->LoadFeaturesMaxValue();
  this->featuresSize = configParser->LoadFeaturesSize();
  this->thresholdsMinValue = configParser->LoadFeaturesMinValue();
  this->thresholdsMaxValue = configParser->LoadThresholdsMaxValue();
  this->thresholdsSize = configParser->LoadThresholdsSize();
  this->percentImagesPerTree = configParser->LoadPercentImagesPerTree();
  return 0;
}

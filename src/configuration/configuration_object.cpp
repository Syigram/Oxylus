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


int ConfigurationObject::SetImagesCols() {
  this->imagesCols = configParser->LoadImagesCols();
  return 0;
}


int ConfigurationObject::SetImagesRows() {
  this->imagesRows = configParser->LoadImagesRows();
  return 0;
}

int ConfigurationObject::SetImagesNumber() {
  this->imagesNumber = configParser->LoadImagesNumber();
  return 0;
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

int ConfigurationObject::LoadConfiguration()  {
  SetImagesCols();
  SetImagesRows();
  SetImagesNumber();
  return 0;
}

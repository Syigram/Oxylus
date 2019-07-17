#include <string>
#include <iostream>

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/exceptions.hpp>

#include <oxylus/configuration/configuration_constants.h>
#include <oxylus/configuration/configuration_parser.h>

using namespace rdf;

ConfigurationParser::ConfigurationParser() {
  this->filename = rdf::bpc::defaults::CONFIGFILE_PATH;
  LoadConfigFile();
}


ConfigurationParser::ConfigurationParser(std::string filename) {
  this->filename = filename;
  LoadConfigFile();
}

int ConfigurationParser::LoadConfigFile() {
  try {
    BoostTree tree;
    boost::property_tree::read_ini(this->filename, tree);
    this->parseTree = tree;
  }
  catch (boost::property_tree::ini_parser::ini_parser_error &except) {
    /* If the given file is missing, a default config file is created. */
    std::cout << "WARNING: " << except.what() << std::endl;
    std::cout << "INFO: Writing default values as: " << this->filename << std::endl;
    WriteDefaultConfigurationFile();
  }
  return 0;
}

std::string ConfigurationParser::LoadDepthFolder(){
  std::string placeholder = "";
  try{
    placeholder = this->parseTree.get<std::string>("images.depthFolder");
  } catch (const boost::property_tree::ptree_error &e){
    placeholder = e.what();
    std::cout << placeholder << std::endl;
  }
  return placeholder;
}

std::string ConfigurationParser::LoadBaseFolder(){
  std::string placeholder = "";
  try{
    placeholder = this->parseTree.get<std::string>("images.baseFolder");
  } catch (const boost::property_tree::ptree_error &e){
    placeholder = e.what();
    std::cout << placeholder << std::endl;
  }
  return placeholder;
}

std::string ConfigurationParser::LoadLabelFolder(){
  std::string placeholder = "";
  try{
    placeholder = this->parseTree.get<std::string>("images.labelFolder");
  } catch (const boost::property_tree::ptree_error &e){
    placeholder = e.what();
    std::cout << placeholder << std::endl;
  }
  return placeholder;
}

std::string ConfigurationParser::LoadFileExtension(){
  std::string placeholder = "";
  try{
    placeholder = this->parseTree.get<std::string>("images.fileExtension");
  } catch (const boost::property_tree::ptree_error &e){
    placeholder = e.what();
    std::cout << placeholder << std::endl;
  }
  return placeholder;
}

std::string ConfigurationParser::LoadFilePrefix(){
  std::string placeholder = "";
  try{
    placeholder = this->parseTree.get<std::string>("images.filePrefix");
  } catch (const boost::property_tree::ptree_error &e){
    placeholder = e.what();
    std::cout << placeholder << std::endl;
  }
  return placeholder;
}

std::string ConfigurationParser::LoadOutputFolder(){
  std::string placeholder = "";
  try{
    placeholder = this->parseTree.get<std::string>("images.outputFolder");
  } catch (const boost::property_tree::ptree_error &e){
    placeholder = e.what();
    std::cout << placeholder << std::endl;
  }
  return placeholder;
}

int ConfigurationParser::LoadPointsSize(){
  int value = 0;
  try{
    value = this->parseTree.get<int>("points.sampleSize");
  }catch (const boost::property_tree::ptree_error &e){
    std::string exception = e.what();
    std::cout << exception << std::endl;
  }
  return value;
}

int ConfigurationParser::LoadMemoryUsage(){
  int value = 0;
  try{
    value = this->parseTree.get<int>("memory.usage");
  }catch (const boost::property_tree::ptree_error &e){
    std::string exception = e.what();
    std::cout << exception << std::endl;
  }
  return value;
}

int ConfigurationParser::LoadTrainingStopCondition(){
  int value = 0;
  try{
    value = this->parseTree.get<int>("hyperparameters.stopCondition");
  }catch (const boost::property_tree::ptree_error &e){
    std::string exception = e.what();
    std::cout << exception << std::endl;
  }
  return value;
}

int ConfigurationParser::LoadNumberOfTrees(){
  int value = 0;
  try{
    value = this->parseTree.get<int>("hyperparameters.trees");
  }catch (const boost::property_tree::ptree_error &e){
    std::string exception = e.what();
    std::cout << exception << std::endl;
  }
  return value;
}

int ConfigurationParser::LoadImagesPerNode(){
  int value = 0;
  try{
    value = this->parseTree.get<int>("hyperparameters.imagesPerNode");
  }catch (const boost::property_tree::ptree_error &e){
    std::string exception = e.what();
    std::cout << exception << std::endl;
  }
  return value;
}

int ConfigurationParser::LoadFeaturesMinValue(){
  int value = 0;
  try{
    value = this->parseTree.get<int>("hyperparameters.featuresMinValue");
  }catch (const boost::property_tree::ptree_error &e){
    std::string exception = e.what();
    std::cout << exception << std::endl;
  }
  return value;
}

int ConfigurationParser::LoadDeltaMinXOffset(){
  int value = 0;
  try{
    value = this->parseTree.get<int>("hyperparameters.deltaMinXOffset");
  }catch (const boost::property_tree::ptree_error &e){
    std::string exception = e.what();
    std::cout << exception << std::endl;
  }
  return value;
}

int ConfigurationParser::LoadDeltaMaxXOffset(){
  int value = 0;
  try{
    value = this->parseTree.get<int>("hyperparameters.deltaMaxXOffset");
  }catch (const boost::property_tree::ptree_error &e){
    std::string exception = e.what();
    std::cout << exception << std::endl;
  }
  return value;
}

int ConfigurationParser::LoadDeltaMinYOffset(){
  int value = 0;
  try{
    value = this->parseTree.get<int>("hyperparameters.deltaMinYOffset");
  }catch (const boost::property_tree::ptree_error &e){
    std::string exception = e.what();
    std::cout << exception << std::endl;
  }
  return value;
}

int ConfigurationParser::LoadDeltaMaxYOffset(){
  int value = 0;
  try{
    value = this->parseTree.get<int>("hyperparameters.deltaMaxYOffset");
  }catch (const boost::property_tree::ptree_error &e){
    std::string exception = e.what();
    std::cout << exception << std::endl;
  }
  return value;
}

int ConfigurationParser::LoadFeaturesMaxValue(){
  int value = 0;
  try{
    value = this->parseTree.get<int>("hyperparameters.featuresMaxValue");
  }catch (const boost::property_tree::ptree_error &e){
    std::string exception = e.what();
    std::cout << exception << std::endl;
  }
  return value;
}

int ConfigurationParser::LoadThresholdsMinValue(){
  int value = 0;
  try{
    value = this->parseTree.get<int>("hyperparameters.thresholdsMinValue");
  }catch (const boost::property_tree::ptree_error &e){
    std::string exception = e.what();
    std::cout << exception << std::endl;
  }
  return value;
}

int ConfigurationParser::LoadThresholdsMaxValue(){
  int value = 0;
  try{
    value = this->parseTree.get<int>("hyperparameters.thresholdsMaxValue");
  }catch (const boost::property_tree::ptree_error &e){
    std::string exception = e.what();
    std::cout << exception << std::endl;
  }
  return value;
}

int ConfigurationParser::LoadFeaturesSize(){
  int value = 0;
  try{
    value = this->parseTree.get<int>("hyperparameters.featuresSize");
  }catch (const boost::property_tree::ptree_error &e){
    std::string exception = e.what();
    std::cout << exception << std::endl;
  }
  return value;
}

int ConfigurationParser::LoadThresholdsSize(){
  int value = 0;
  try{
    value = this->parseTree.get<int>("hyperparameters.thresholdsSize");
  }catch (const boost::property_tree::ptree_error &e){
    std::string exception = e.what();
    std::cout << exception << std::endl;
  }
  return value;
}

int ConfigurationParser::LoadMaxTreeLevels(){
  int value = 0;
  try{
    value = this->parseTree.get<int>("hyperparameters.maxLevels");
  }catch (const boost::property_tree::ptree_error &e){
    std::string exception = e.what();
    std::cout << exception << std::endl;
  }
  return value;
}

int ConfigurationParser::LoadPercentImagesPerTree(){
  int value = 0;
  try{
    value = this->parseTree.get<int>("hyperparameters.imagesPerTree");
  }catch (const boost::property_tree::ptree_error &e){
    std::string exception = e.what();
    std::cout << exception << std::endl;
  }
  return value;
}

int ConfigurationParser::LoadImagesCols() {
  return this->parseTree.get<int>("images.rows");
}

int ConfigurationParser::LoadImagesRows() {
  return this->parseTree.get<int>("images.cols");
}

int ConfigurationParser::LoadImagesNumber() {
  return this->parseTree.get<int>("images.images");
}


int ConfigurationParser::WriteDefaultConfigurationFile() {

  BoostTree tree;

  WriteDefaultImagesSection(tree);

  boost::property_tree::write_ini(this->filename, tree);

  this->parseTree = tree;


  return 0;

}

int ConfigurationParser::WriteDefaultImagesSection(BoostTree& tree) {
  const std::string sectionName = "images."; /* trailing dot is default
                                                Boost behaviour managing
                                                INI files mappings*/
  const std::string rowsName = "rows";
  const std::string colsName = "cols";
  const std::string imagesNumberName = "images";

  tree.put(sectionName + rowsName, rdf::bpc::defaults::IMAGES_ROWS);
  tree.put(sectionName + colsName, rdf::bpc::defaults::IMAGES_COLS);
  tree.put(sectionName + imagesNumberName, rdf::bpc::defaults::IMAGES_NUMBER);

  return 0;
}

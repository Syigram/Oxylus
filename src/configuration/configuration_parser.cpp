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

}

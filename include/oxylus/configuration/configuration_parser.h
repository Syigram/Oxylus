#ifndef CONFIGURATION_PARSER_H
#define CONFIGURATION_PARSER_H

#include <string>

#include <boost/property_tree/ptree.hpp>

namespace rdf  {

  typedef boost::property_tree::ptree BoostTree;

  class ConfigurationParser {
    private:
      std::string filename;
      BoostTree parseTree;
      int WriteDefaultImagesSection(BoostTree& tree);
    public:
      ConfigurationParser();
      ConfigurationParser(const std::string filename);
      int LoadConfigFile();
      int LoadImagesCols();
      int LoadImagesRows();
      int LoadImagesNumber();
      int WriteDefaultConfigurationFile();
      ~ConfigurationParser(){};

  };

}

#endif /* CONFIGURATION_PARSER_H */

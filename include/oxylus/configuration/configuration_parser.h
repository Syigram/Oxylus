#ifndef CONFIGURATION_PARSER_H

#define CONFIGURATION_PARSER_H

#include <string>

#include <boost/property_tree/ptree.hpp>

#include <oxylus/memory/helper.h>

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
      int LoadPointsSize();
      int LoadMemoryUsage();
      int LoadNumberOfTrees();
      int LoadMaxTreeLevels();
      int LoadImagesPerNode();
      int LoadThresholdsMinValue();
      int LoadThresholdsMaxValue();
      int LoadFeaturesMinValue();
      int LoadFeaturesMaxValue();
      std::string LoadBaseFolder();
      std::string LoadLabelFolder();
      std::string LoadDepthFolder();
      std::string LoadFileExtension();
      std::string LoadFilePrefix();
      int WriteDefaultConfigurationFile();
      ~ConfigurationParser(){};

  };

}

#endif /* CONFIGURATION_PARSER_H */

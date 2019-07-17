#ifndef CONFIGURATION_OBJECT_H
#define CONFIGURATION_OBJECT_H


#include <oxylus/configuration/configuration_parser.h>

namespace rdf  {

  class ConfigurationObject {
    public:
      ConfigurationObject();
      ConfigurationObject(std::string configurationFile);
      int GetImagesRows();
      int GetImagesCols();
      int GetImagesNumber();
      int GetPointsSize();
      int GetMemoryUsage();
      int GetNumberOfTrees();
      int GetMaxTreeLevels();
      int GetStopCondition();
      int GetImagesPerNode();
      int GetFeaturesMinValue();
      int GetFeaturesMaxValue();
      int GetFeaturesSize();
      int GetThresholdsMinValue();
      int GetThresholdsMaxValue();
      int GetThresholdsSize();
      int GetPercentImagesPerTree();
      int GetDeltaMinXOffset();
      int GetDeltaMaxXOffset();
      int GetDeltaMinYOffset();
      int GetDeltaMaxYOffset();
      std::string GetBaseFolder();
      std::string GetLabelFolder();
      std::string GetDepthFolder();
      std::string GetFileExtension();
      std::string GetFilePrefix();
      std::string GetOutputFolder();
      ~ConfigurationObject(){};
    private:
      std::shared_ptr<ConfigurationParser> configParser;
      int imagesRows;
      int imagesCols;
      int imagesNumber;
      int pointsSize;
      int memoryUsage;
      int numberOfTrees;
      int stopCondition;
      int maxTreeLevels;
      int imagesPerNode;
      int featuresMinValue;
      int featuresMaxValue;
      int featuresSize;
      int thresholdsMinValue;
      int thresholdsMaxValue;
      int thresholdsSize;
      int percentImagesPerTree;
      int deltaMinXOffset;
      int deltaMaxXOffset;
      int deltaMinYOffset;
      int deltaMaxYOffset;
      std::string baseFolder;
      std::string labelFolder;
      std::string depthFolder;
      std::string fileExtension;
      std::string filePrefix;
      std::string outputFolder;
      int LoadConfiguration();
  };

} /* rdf */

#endif /* CONFIGURATION_OBJECT_H */

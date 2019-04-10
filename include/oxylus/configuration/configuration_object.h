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
      std::string GetBaseFolder();
      std::string GetLabelFolder();
      std::string GetDepthFolder();
      std::string GetFileExtension();
      std::string GetFilePrefix();
      ~ConfigurationObject(){};
    private:
      std::shared_ptr<ConfigurationParser> configParser;
      int imagesRows;
      int imagesCols;
      int imagesNumber;
      std::string baseFolder;
      std::string labelFolder;
      std::string depthFolder;
      std::string fileExtension;
      std::string filePrefix;
      int LoadConfiguration();
  };

} /* rdf */

#endif /* CONFIGURATION_OBJECT_H */

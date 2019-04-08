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
      ~ConfigurationObject(){};

    private:
      std::shared_ptr<ConfigurationParser> configParser;
      int imagesRows;
      int imagesCols;
      int imagesNumber;

      int LoadConfiguration();
      int SetImagesRows();
      int SetImagesCols();
      int SetImagesNumber();
  };

} /* rdf */

#endif /* CONFIGURATION_OBJECT_H */

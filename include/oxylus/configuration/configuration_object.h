#ifndef PROGRAM_CONFIG_H
#define PROGRAM_CONFIG_H

#include <oxylus/configuration/config_parser.h>

class ProgramConfiguration {
  public:
    ProgramConfiguration();
    ProgramConfiguration(std::string configurationFile);
    ~ProgramConfiguration(){};

  private:
    int imagesRows;
    int imagesCols;
    int imagesNumber;


    ConfigParser configParser;
}

#endif /* PROGRAM_CONFI

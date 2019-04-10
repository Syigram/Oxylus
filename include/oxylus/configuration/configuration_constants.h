
#ifndef CONFIGURATION_CONSTANTS_H
#define CONFIGURATION_CONSTANTS_H

#include <string>

namespace rdf  {
  namespace bpc  {
    namespace defaults {

      const std::string CONFIGFILE_PATH = "/home/agonzalez/tfg/oxylus_github/build/config.ini";
      const int IMAGES_ROWS = 300;
      const int IMAGES_COLS = 200;
      const int IMAGES_NUMBER = 100;

    }


    namespace constants  {


    const int IMAGE_AVG_SIZE = 100;


    const int BLACK = 0;
    const int BLUE = 1;
    const int GREEN = 2;
    const int CIAN = 3;
    const int RED = 4;
    const int MAGENTA = 5;
    const int YELLOW = 6;
    const int WHITE = 7;

    }
  }
}

#endif

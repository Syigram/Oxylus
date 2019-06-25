
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
    const int PALETTE_SIZE = 8;
    const int BODY_PARTS = 8;


    const int BLACK = 0;
    const int BLUE = 1;
    const int GREEN = 2;
    const int CIAN = 3;
    const int RED = 4;
    const int MAGENTA = 5;
    const int YELLOW = 6;
    const int WHITE = 7;

    const int HIGH_VALUE = 2147483647;

    }

    namespace hand{

      using namespace rdf::bpc::constants;

      const int THUMB_FINGER = WHITE;
      const int INDEX_FINGER = RED;
      const int MIDDLE_FINGER = GREEN;
      const int RING_FINGER = BLUE;
      const int PINKY_FINGER = CIAN;
      const int PALM = YELLOW;
      const int FOREARM = MAGENTA;
      const int BACKGROUND = BLACK;

    }
  }
}

#endif

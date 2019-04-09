#ifndef IMAGE_OPERATIONS_H
#define IMAGE_OPERATIONS_H

#include <map>
#include <set>
#include <vector>
#include <random>

#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <oxylus/image/image_structure.h>

namespace rdf  {
  namespace bpc  {

    typedef std::map<cv::Vec3b,uchar> MapPalette;


    namespace cv {
      /**
       * Simple less comparator for Vec3b to be able to use maps
       */
      inline bool operator<(const Vec3b& a,const Vec3b& b) {
        return (a[0]< b[0]) ||
          ((a[0]==b[0]) && ( (a[1]< b[1]) ||
                             ( (a[1]==b[1]) && (a[2]< b[2]))));
      }
    }

    class ImageOperations {
      public:
        ImageOperations();
        static MapPalette InitializePalette();
        static void PrintPalette(MapPalette palette);
        static std::set<std::pair<int,int>> RandomPointsSelection(int rows, int cols, int n);
        static PointsVector GenerateRandomPoints(ImageStructure imageData);
        static PointStructure GenerateRandomPointStructure();
        virtual ~ImageOperations(){};

      private:

        //<! Static function for initializing seed for random distribution.
        //<! It is only initialized once.
        static std::mt19937 InitRandomSeed();

        //<! MersenneTwister pseudo-random engine
        static std::mt19937 mt_;


    };
  }
}


#endif

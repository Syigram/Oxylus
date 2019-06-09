#ifndef IMAGE_OPERATIONS_H
#define IMAGE_OPERATIONS_H

#include <map>
#include <utility>
#include <set>
#include <vector>
#include <random>
#include <memory>
#include <algorithm>
#include <functional>

#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <oxylus/configuration/configuration_object.h>
#include <oxylus/image/image_structure.h>


namespace cv {
  /**
   * Simple less comparator for Vec3b to be able to use maps
   */
  inline bool operator<(const Vec3b& a,const Vec3b& b) {
    return (a[0]< b[0]) ||
      ((a[0]==b[0]) && ( (a[1]< b[1]) ||
                         ( (a[1]==b[1]) && (a[2]< b[2]))));
  }

  /**
   * Simple less comparator for Point to be able to use maps
   */
  inline bool operator<(const Point& a, const Point& b){
    return ( a.x < b.x && a.y<b.y);
  }

}



namespace rdf  {
  namespace bpc  {

    typedef std::map<cv::Vec3b,uchar> MapPalette;
    typedef std::map<int,std::vector<cv::Point>> MapColorsHistogram;
    typedef std::set<std::pair<int,std::vector<cv::Point>>> SetColorsHistogram;
    typedef std::vector<std::pair<int,std::vector<cv::Point>>> VecColorsHistogram;


    class ImageOperations {
      public:
        ImageOperations();
        ImageOperations(ConfigurationObject* configurationObject);
        static MapPalette InitializePalette();
        static void PrintPalette(MapPalette palette);
        static void SavePaletteTo(std::string filepath, MapPalette& palette);
        static std::set<std::pair<int,int>> RandomPointsSelection(int rows, int cols, int n);
        static std::shared_ptr<PointStructureVec> GenerateRandomPoints(ImageStructure& imageData,
                                                 cv::Mat_<ushort>& depthImage,
                                                 cv::Mat_<uchar>& labelImage,
                                                 MapPalette palette);
        static PointStructure GenerateRandomPointStructure();
        static int GetNumberOfPointsForNode(int nodeId, std::shared_ptr<ImagesVector> imagesVec);
        int AssignImagesToTrees(std::shared_ptr<ImagesVector> imagesVector);
        virtual ~ImageOperations(){};

      private:

        //<! Static function for initializing seed for random distribution.
        //<! It is only initialized once.
        static std::mt19937 InitRandomSeed();
        int treesPercent;
        int numOfTrees;
        static VecColorsHistogram GenerateColorsHistogramForImage(
            cv::Mat_<uchar>& labelImage,
            int rows,
            int cols
            );

        static VecColorsHistogram SortColorsHistogram(MapColorsHistogram& mapColorsHistogram);

        static bool PointExistsInVector(std::vector<cv::Point> pointsVector, cv::Point point);
        /* static MapPalette palette; */
        //<! MersenneTwister pseudo-random engine
        static std::mt19937 mt_;

    };
  }
}


#endif

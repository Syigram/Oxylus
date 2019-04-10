#ifndef IMAGE_STRUCTURE_H
#define IMAGE_STRUCTURE_H

/* #include <map> */
#include <set>
#include <vector>
#include <memory>

#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>

#include <oxylus/configuration/configuration_object.h>
#include <oxylus/image/point_structure.h>

#define KILOBYTE 1024

namespace rdf  {

  namespace bpc  {

    class ImageStructure {
      public:
        ImageStructure();
        ImageStructure(ConfigurationObject* configObject);
        ImageStructure(ConfigurationObject* configObject, int imageId);
        int GetRows();
        int GetCols();
        int GetPointsSize();
        int GetImageId();
        int GetSizeOf();
        void SetPointsVector(std::shared_ptr<PointsVector> pointsVector);
        std::shared_ptr<PointsVector> GetPointsVector();
      private:
        /* data */
        cv::Mat_<uchar> labelImage;
        cv::Mat_<ushort> depthImage;

        std::vector<int> treesId; /* this vector contains the id of the trees that use
                                this image for training */

        std::set<std::pair<int,int>> setOfPoints;
        std::shared_ptr<PointsVector> pointsVector;
        /* std::string labefilename; */
        int imageId;
        int rows;
        int cols;
        int pointsSize;
    };
    typedef std::vector<ImageStructure> ImagesVector;
  }

}

#endif /* IMAGE_STRUCTURE_H */

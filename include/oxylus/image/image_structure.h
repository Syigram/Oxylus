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
    
    class ImageStructure;

    typedef std::vector<ImageStructure> ImagesVector;

    class ImageStructure {
      public:
        ImageStructure();
        ImageStructure(ConfigurationObject* configObject);
        ImageStructure(ConfigurationObject* configObject, int imageId);
        ImageStructure(ConfigurationObject* configObject, cv::Mat_<ushort> depthImage,  int imageId);
        int GetRows();
        int GetCols();
        int GetPointsSize();
        int GetImageId();
        int GetSizeOf(ConfigurationObject* configObject);
        void SetPointsVector(std::shared_ptr<PointStructureVec> pointsVector);
        void SetDepthImage(cv::Mat_<ushort> depthImage);
        std::shared_ptr<PointStructureVec> GetPointsVector();
        std::vector<int> treesId; /* this vector contains the id of the trees that use
                                this image for training */
        std::shared_ptr<PointStructureVec> pointsVector;
      private:
        /* data */
        cv::Mat_<ushort> depthImage;

        /* std::string labefilename; */
        int imageId;
        int rows;
        int cols;
        int pointsSize;
        int treesSize;
    };

  }

}

#endif /* IMAGE_STRUCTURE_H */

#ifndef POINT_STRUCTURE_H
#define POINT_STRUCTURE_H

#include <vector>

#include <opencv2/core/types.hpp>

#include <oxylus/image/point_structure.h>


namespace rdf {
  namespace bpc {

    class PointStructure;

    typedef std::vector<PointStructure> PointStructureVec;

    class PointStructure {
      public:
        PointStructure();
        PointStructure(int node, int labelValue, int x, int y, int imageId);
        PointStructure(cv::Point point, int labelValue, int imageId, int node);
        void SetImageId(int imageId);
        void SetCurrentNode(int node);
        void SetLabelPixelValue(int value);
        void SetPoint(int x, int y);
        void SetX(int x);
        void SetY(int y);
        int GetCurrentNode();
        int GetLabelPixelValue();
        int GetX();
        int GetY();
        cv::Point GetPoint();
        int GetImageId();
        void Print();
        ~PointStructure(){};

      private:
        int currentNode;
        int labelPixelValue;
        cv::Point point;
        int imageId;
    };

  }
}


#endif /* POINT_STRUCTURE_H */

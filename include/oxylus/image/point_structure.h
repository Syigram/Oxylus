#ifndef POINT_STRUCTURE_H
#define POINT_STRUCTURE_H

namespace rdf {
  namespace bpc {

    typedef vector<PointStructure> PointsVector;

    class PointStructure {
      public:
        PointStructure();
        PointStructure(int node, int depthValue, int labelValue, int x, int y, int imageId);
        void SetImageId(int imageId);
        void SetCurrentNode(int node);
        void SetDepthPixelValue(int value);
        void SetLabelPixelValue(int value);
        void SetX(int x);
        void SetY(int y);
        int GetCurrentNode();
        int GetLabelPixelValue();
        int GetDepthPixelValue();
        int GetX();
        int GetY();
        int GetImageId();
        ~PointStructure(){};

      private:
        int currentNode;
        int depthPixelValue;
        int labelPixelValue;
        int x;
        int y;
        int imageId;
    }
  }
}


#endif /* POINT_STRUCTURE_H */

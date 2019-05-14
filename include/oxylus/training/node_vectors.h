#ifndef NODE_VECTORS_H
#define NODE_VECTORS_H

#include <vector>

#include <oxylus/training/features.h>

#include <oxylus/configuration/Serialization.h>

namespace rdf {
  namespace bpc {

    class NodeVectors{
      private:

        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version) {
          ar & thresholdsVec;
          ar & featuresVec;
          ar & nodeId;
        }

      public:
        NodeVectors(){};
        NodeVectors(int nodeId, std::vector<Features>* featuresVec, std::vector<int>* thresholdsVec){
          this->nodeId = nodeId;
          this->thresholdsVec = thresholdsVec;
          this->featuresVec = featuresVec;
        }
        int nodeId;
        std::vector<int>* thresholdsVec;
        std::vector<Features>* featuresVec;
        virtual ~NodeVectors(){};
    };
  }
}


#endif /* NODE_VECTORS_H */

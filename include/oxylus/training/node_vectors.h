#ifndef NODE_VECTORS_H
#define NODE_VECTORS_H

#include <vector>

#include <oxylus/configuration/Serialization.h>
#include <oxylus/training/matrix.h>
#include <oxylus/training/cell.h>
#include <oxylus/training/features.h>


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
          ar & nodeHistograms;
        }

      public:
        NodeVectors(){};
        NodeVectors(int nodeId, std::vector<Features>* featuresVec, std::vector<int>* thresholdsVec){
          this->nodeId = nodeId;
          this->thresholdsVec = thresholdsVec;
          this->featuresVec = featuresVec;
        }
        NodeVectors(const NodeVectors& other) {
          this->nodeId = other.nodeId;
          this->featuresVec = other.featuresVec;
          this->thresholdsVec = other.thresholdsVec;
          this->nodeHistograms = other.nodeHistograms;
        }
        NodeVectors& operator=(const NodeVectors& rhs) {
          this->nodeId = rhs.nodeId;
          this->featuresVec = rhs.featuresVec;
          this->thresholdsVec = rhs.thresholdsVec;
          this->nodeHistograms = rhs.nodeHistograms;
          return *this;
        }

        void PrintLeftHistograms() {
          for (int i = 0; i < featuresVec->size(); ++i) {
            for (int j = 0; j < thresholdsVec->size(); ++j) {
              std::cout << "nodeVector[0]:" <<  nodeHistograms[i][j].leftHistogram[0] << std::endl;        
              std::cout << "nodeVector[1]:" <<  nodeHistograms[i][j].leftHistogram[1] << std::endl;        
              std::cout << "nodeVector[2]:" <<  nodeHistograms[i][j].leftHistogram[2] << std::endl;        
              std::cout << "nodeVector[3]:" <<  nodeHistograms[i][j].leftHistogram[3] << std::endl;        
              std::cout << "nodeVector[4]:" <<  nodeHistograms[i][j].leftHistogram[4] << std::endl;        
              std::cout << "nodeVector[5]:" <<  nodeHistograms[i][j].leftHistogram[5] << std::endl;        
              std::cout << "nodeVector[6]:" <<  nodeHistograms[i][j].leftHistogram[6] << std::endl;        
            }
          }
        }

        void PrintRightHistograms() {
          for (int i = 0; i < featuresVec->size(); ++i) {
            for (int j = 0; j < thresholdsVec->size(); ++j) {
              std::cout << "nodeVector[0]:" <<  nodeHistograms[i][j].rightHistogram[0] << std::endl;        
              std::cout << "nodeVector[1]:" <<  nodeHistograms[i][j].rightHistogram[1] << std::endl;        
              std::cout << "nodeVector[2]:" <<  nodeHistograms[i][j].rightHistogram[2] << std::endl;        
              std::cout << "nodeVector[3]:" <<  nodeHistograms[i][j].rightHistogram[3] << std::endl;        
              std::cout << "nodeVector[4]:" <<  nodeHistograms[i][j].rightHistogram[4] << std::endl;        
              std::cout << "nodeVector[5]:" <<  nodeHistograms[i][j].rightHistogram[5] << std::endl;        
              std::cout << "nodeVector[6]:" <<  nodeHistograms[i][j].rightHistogram[6] << std::endl;        
            }
          }
        }

        int nodeId;
        std::vector<int>* thresholdsVec;
        std::vector<Features>* featuresVec;
        Matrix<Cell> nodeHistograms;
        virtual ~NodeVectors(){
          /* delete thresholdsVec; */
          /* delete featuresVec; */
        };
    };
  }
}


#endif /* NODE_VECTORS_H */

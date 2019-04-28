#ifndef NODEMATRIX_H
#define NODEMATRIX_H

#include <oxylus/configuration/Serialization.h>
#include <boost/serialization/split_member.hpp>

#include <oxylus/training/matrix.h>
#include <oxylus/training/cell.h>
#include <oxylus/training/features.h>

#include <assert.h>

namespace rdf {
  namespace bpc {
     class NodeMatrix{


       public:

         int featuresSize;
         int thresholdsSize;

         Matrix<Cell>* cellHistograms;
         std::vector<Features>* featuresVector;
         std::vector<int>* thresholdsVector;

         NodeMatrix();
         void SetFeaturesVector(std::vector<Features>* featuresVector){
           this->featuresSize = featuresVector->size();
           this->featuresVector = featuresVector;
         }
         void SetThresholdsVector(std::vector<int>* thresholdsVector){
           this->thresholdsSize = thresholdsVector->size();
           this->thresholdsVector = thresholdsVector;
         }

         /* std::vector<int>* GetThresholdsVector() { */
         /*   return this->thresholdsVector; */
         /* } */

         /* std::vector<Features>* GetFeaturesVector() { */
         /*   return this->featuresVector; */
         /* } */

         Matrix<Cell>* GetCellsHistograms() {
           return this->cellHistograms;
         }

         void InitCellHistograms(std::vector<int>* thresholdsVector, std::vector<Features>* featuresVector){
           this->thresholdsSize = thresholdsVector->size();
           this->featuresSize = featuresVector->size();
           this->SetThresholdsVector(thresholdsVector);
           this->SetFeaturesVector(featuresVector);
         }

         void InitCellHistograms(){
           std::cout << "size features from init" << featuresSize << std::endl;
           std::cout << "size thresh from init" << thresholdsSize << std::endl;
           Matrix<Cell>* cellsMatrix = new Matrix<Cell>(this->featuresSize, this->thresholdsSize);
           this->cellHistograms = cellsMatrix;
         }

         /* void PrintFeaturesVector(){ */
         /*   for (auto& it: featuresVector){ */
         /*    it.Print(); */
         /*   } */
         /* } */

         /* void PrintThresholdsVector(){ */
         /*   for (auto& it: thresholdsVector){ */
         /*     std::cout << it << std::endl; */
         /*   } */

         /* } */

         /* void PrintCellHistograms(){ */
         /*   int count = 0; */
         /*   for (auto& it: cellHistograms){ */
         /*     std::cout << count++ << "\n"; */
         /*   } */
         /* } */

        inline bool operator==(NodeMatrix &rhs){
          if (this->featuresSize != rhs.featuresSize ||
              this->thresholdsSize != rhs.thresholdsSize){
            return false;
          }
          std::cout << "same size" << std::endl;
          for (int i = 0; i < this->thresholdsSize; i++){
            if (this->thresholdsVector[i] != rhs.thresholdsVector[i]){
              return false;
            }
          }
          std::cout << "same thresholds" << std::endl;
          for (int i = 0; i < this->featuresSize; i++){
            if (this->featuresVector->at(i) != rhs.featuresVector->at(i)){
              return false;
            }
          }
          std::cout << "same features" << std::endl;
          for (int i = 0; i < this->featuresSize; i++){
            for (int j = 0; j < this->thresholdsSize; j++){
              if ((*cellHistograms)(i,j) != rhs.cellHistograms->operator()(i,j)){
                return false;
              }
            }
          }
          std::cout << "same cells" << std::endl;
          return true;
         }

        ~NodeMatrix(){
          delete[] featuresVector;
          delete[] thresholdsVector;
          delete cellHistograms;

        }

         /* ThresholdsVectorPointer thresholdsVector; */

      private:


        /* --- Serialization --- */
          friend class boost::serialization::access;

          template<class Archive>
          void save(Archive & ar, const unsigned int version) const {
            ar & featuresSize;
            ar & thresholdsSize;
            ar & featuresVector;
            ar & thresholdsVector;
            ar & cellHistograms;

          }

          template<class Archive>
          void load(Archive & ar, const unsigned int version) {
            ar & featuresSize;
            ar & thresholdsSize;
            ar & featuresVector;
            std::cout << "thresh size from load: " << thresholdsSize;
            std::cout << "feat size from load: " << featuresSize;
            ar & thresholdsVector;
            InitCellHistograms();
            ar & cellHistograms;

          }
          BOOST_SERIALIZATION_SPLIT_MEMBER()
          /* ==== Serialization ==== */


     };

     NodeMatrix::NodeMatrix(){

     }


  }
}

#endif /* NODEMATRIX_H */

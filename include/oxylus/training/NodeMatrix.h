#ifndef NODEMATRIX_H
#define NODEMATRIX_H

#include <oxylus/configuration/Serialization.h>

#include <oxylus/training/matrix.h>
#include <oxylus/training/cell.h>
#include <oxylus/training/features.h>

#include <assert.h>

namespace rdf {
  namespace bpc {
     class NodeMatrix{


       public:

         size_t featuresSize;
         size_t thresholdsSize;

         Matrix<Cell> cellHistograms;
         std::vector<Features>* featuresVector;
         std::vector<int>* thresholdsVector;

         NodeMatrix();
         NodeMatrix(const NodeMatrix& other);
         /* void SetFeaturesVector( std::vector<Features> featuresVector){ */
         /*   this->featuresSize = featuresVector.size(); */
         /*   this->featuresVector = featuresVector; */
         /* } */
         /* void SetThresholdsVector(std::vector<int> thresholdsVector){ */
         /*   this->thresholdsSize = thresholdsVector.size(); */
         /*   this->thresholdsVector = thresholdsVector; */
         /* } */


         int GetThresholdsSize() const {
           return this->thresholdsSize;
         }

         int GetFeaturesSize() const {
           return this->featuresSize;
         }

         Matrix<Cell> GetCellHistograms() const {
           return this->cellHistograms;
         }
         void SetFeaturesVector(std::vector<Features>* featuresVector){
           this->featuresSize = featuresVector->size();
           this->featuresVector = featuresVector;
         }
         void SetThresholdsVector(std::vector<int>* thresholdsVector){
           this->thresholdsSize = thresholdsVector->size();
           this->thresholdsVector = thresholdsVector;
         }

         std::vector<int>* GetThresholdsVector() const {
           return this->thresholdsVector;
         }

         std::vector<Features>* GetFeaturesVector() const {
           return this->featuresVector;
         }

         /* void InitCellHistograms(std::vector<int>* thresholdsVector, std::vector<Features>* featuresVector){ */
         /*   this->thresholdsSize = thresholdsVector->size(); */
         /*   this->featuresSize = featuresVector->size(); */
         /*   this->SetThresholdsVector(thresholdsVector); */
         /*   this->SetFeaturesVector(featuresVector); */
         /* } */

         void SetCellHistogram(Matrix<Cell> cellHistograms){
           this->cellHistograms = cellHistograms;
         }

         void InitCellHistograms(int rows, int cols){
           std::cout << "size features from init" << featuresSize << std::endl;
           std::cout << "size thresh from init" << thresholdsSize << std::endl;
           Cell emptyCell;
           Matrix<Cell> cellsMatrix(10, 10, Cell() );
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

        inline bool operator=(NodeMatrix& other){
         this->featuresSize = other.GetFeaturesSize();
         this->thresholdsSize = other.GetThresholdsSize();
         this->featuresVector = other.GetFeaturesVector();
         this->thresholdsVector = other.GetThresholdsVector();
         this->cellHistograms = other.GetCellHistograms();
         return true;
        }

        inline bool operator==(NodeMatrix &rhs){
          if (this->featuresSize != rhs.featuresSize ||
              this->thresholdsSize != rhs.thresholdsSize){
            return false;
          }
          for (size_t i = 0; i < this->thresholdsSize; i++){
            if (this->thresholdsVector->at(i) != rhs.thresholdsVector->at(i)){
              return false;
            }
          }
          for (size_t i = 0; i < this->featuresSize; i++){
            if (this->featuresVector->at(i) != rhs.featuresVector->at(i)){
              return false;
            }
          }
          size_t histogramsCols = this->cellHistograms.rows();
          size_t histogramsRows = this->cellHistograms.cols();
          for (size_t i = 0; i < histogramsRows; i++){
            for (size_t j = 0; j < histogramsCols; j++){
              if (cellHistograms(i, j) != rhs.cellHistograms(i, j)){
                return false;
              }
            }
          }
          return true;
         }

        ~NodeMatrix(){
          /* delete[] featuresVector; */
          /* delete[] thresholdsVector; */
          /* delete cellHistograms; */

        }

         /* ThresholdsVectorPointer thresholdsVector; */

      private:


        /* --- Serialization --- */
          friend class boost::serialization::access;

          template<class Archive>
          void serialize(Archive & ar, const unsigned int version) {
            ar & featuresSize;
            std::cout << "serailizing fetsize.. " << std::endl;
            ar & thresholdsSize;
            std::cout << "serailizing threshsize.. " << std::endl;
            ar & featuresVector;
            std::cout << "serailizing featvect.. " << std::endl;
            ar & thresholdsVector;
            std::cout << "serailizing thresholdsVec.. " << std::endl;
            ar & cellHistograms;
            std::cout << "serailizing cellHistograms.. " << std::endl;
          }

          /* template<class Archive> */
          /* void load(Archive & ar, const unsigned int version) { */
          /*   ar & featuresSize; */
          /*   ar & thresholdsSize; */
          /*   ar & featuresVector; */
          /*   std::cout << "thresh size from load: " << thresholdsSize; */
          /*   std::cout << "feat size from load: " << featuresSize; */
          /*   ar & thresholdsVector; */
          /*   ar & cellHistograms; */

          /* } */
          /* BOOST_SERIALIZATION_SPLIT_MEMBER() */
          /* ==== Serialization ==== */


     };

     NodeMatrix::NodeMatrix(){

     }

     NodeMatrix::NodeMatrix(const NodeMatrix& other){
       this->featuresSize = other.GetFeaturesSize();
       this->thresholdsSize = other.GetThresholdsSize();
       this->featuresVector = other.GetFeaturesVector();
       this->thresholdsVector = other.GetThresholdsVector();
       this->cellHistograms = other.GetCellHistograms();
     }


  }
}

#endif /* NODEMATRIX_H */

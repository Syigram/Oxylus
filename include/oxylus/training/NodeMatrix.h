#ifndef NODEMATRIX_H
#define NODEMATRIX_H

namespace rdf {
  namespace bpc {
     class NodeMatrix{

       typedef std::shared_ptr<std::vector<int>> ThresholdsVectorPointer;
       typedef std::shared_ptr<std::vector<Features>> FeaturesVectorPointer;

       public:
         NodeMatrix();
         SetFeaturesVector(FeaturesVectorPointer featuresVector){
           this->featuresVector = featuresVector;
         }
         SetThresholdsVector(ThresholdsVectorPointer thresholdsVector){
           this->thresholdsVector = thresholdsVector;
         }
         FeaturesVectorPointer featuresVector;
         ThresholdsVectorPointer thresholdsVector;
         Matrix<Cell> matrixHistograms;





     };

     NodeMatrix:::NodeMatrix(){

     }


  }
}

#endif /* NODEMATRIX_H */

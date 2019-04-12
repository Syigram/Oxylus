#ifndef THRESHOLDS_VECTOR_H
#define THRESHOLDS_VECTOR_H

#include <oxylus/memory/helper.h>
#include <oxylus/training/training_vector.h>

namespace rdf{
  namespace bpc{

    class ThresholdsVector{
      public:
        ThresholdsVector();
        ThresholdsVector(size_t size, float max, float min){
          this->size = size;
          this->maxValue = max;
          this->minValue = min;
        }

        std::vector<float> GenerateVector(){
          float randomValue;
          std::vector<float> thresholdsVector;
          std::uniform_real_distribution<float> threshold_dist(minValue, maxValue);
          for (size_t i = 0; i < size; i++){
            randomValue = threshold_dist(mt_);
            thresholdsVector.push_back(randomValue);
          }
          return thresholdsVector;
        }

      private:
        size_t size;
        float maxValue;
        float minValue;
        static std::mt19937 mt_;
    };

    ThresholdsVector::mt_ = Helper::InitializeRandomDevice();

  }
}

#endif /* THRESHOLDS_VECTOR_H */


    /* template <class T1> */
    /* template <class T2> */
    /* class ThresholdsVector : public TrainingVector<T1, T2>{ */
    /*   public: */
    /*     std::vector<T1> GetVector(); */
    /*     void GenerateVector(){ */
    /*       T randomValue; */
    /*       std::uniform_real_distribution<T> threshold_dist(thresholdsLowRange_, thresholdsHighRange_); */

    /*     } */
    /*     void PrintVector(); */

    /*   private: */
    /*     static std::mt19937 mt_; //Mersenne twister pseudo-random engine */
    /* }; */

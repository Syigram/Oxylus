#ifndef THRESHOLDS_VECTOR_H
#define THRESHOLDS_VECTOR_H

#include <iostream>
#include <random>

#include <oxylus/configuration/configuration_object.h>
#include <oxylus/memory/helper.h>

namespace rdf{
  namespace bpc{

    class ThresholdsVectorGenerator{
      public:
        ThresholdsVectorGenerator();
        ThresholdsVectorGenerator(ConfigurationObject* configObject){
          this->minValue = configObject->GetThresholdsMinValue();
          this->maxValue = configObject->GetThresholdsMaxValue();
          this->size = configObject->GetThresholdsSize();
          std::cout << "size from vectorGen" << size << std::endl;
        }
        ThresholdsVectorGenerator(size_t size, int max, int min){
          this->size = size;
          this->maxValue = max;
          this->minValue = min;
        }

        /* std::vector<int> GenerateVector(){ */
        /*   int randomValue; */
        /*   std::vector<int> thresholdsVector; */
        /*   thresholdsVector.reserve(size); */
        /*   std::uniform_int_distribution<int> threshold_dist(minValue, maxValue); */
        /*   for (size_t i = 0; i < size; i++){ */
        /*     randomValue = threshold_dist(mt_); */
        /*     thresholdsVector.push_back(randomValue); */
        /*   } */
        /*   return thresholdsVector; */
        /* } */

        std::vector<int>* GenerateVector(){
          int randomValue;
          /* std::vector<int>* thresholdsVector = new std::vector<int>[size]; */
          std::vector<int>* thresholdsVector = new std::vector<int>();
          thresholdsVector->reserve(size);
          std::uniform_int_distribution<int> threshold_dist(minValue, maxValue);
          for (size_t i = 0; i < size; i++){
            randomValue = threshold_dist(mt_);
            thresholdsVector->push_back(randomValue);
          }
          return thresholdsVector;
        }

        static std::mt19937 mt_;

      private:
        size_t size;
        int maxValue;
        int minValue;
    };

    std::mt19937 ThresholdsVectorGenerator::mt_ = Helper::InitializeRandomDevice();
  }
}

#endif /* THRESHOLDS_VECTOR_H */

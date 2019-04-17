#ifndef THRESHOLDS_VECTOR_H
#define THRESHOLDS_VECTOR_H

#include <oxylus/configuration/configuration_object.h>
#include <oxylus/memory/helper.h>

namespace rdf{
  namespace bpc{

    class ThresholdsVector{
      public:
        ThresholdsVector();
        ThresholdsVector(ConfigurationObject* configObject){
          this->minValue = configObject->GetThresholdsMinValue();
          this->maxValue = configObject->GetThresholdsMaxValue();
        }
        ThresholdsVector(size_t size, int max, int min){
          this->size = size;
          this->maxValue = max;
          this->minValue = min;
        }

        std::vector<int> GenerateVector(){
          int randomValue;
          std::vector<int> thresholdsVector;
          std::uniform_int_distribution<int> threshold_dist(minValue, maxValue);
          for (size_t i = 0; i < size; i++){
            randomValue = threshold_dist(mt_);
            thresholdsVector.push_back(randomValue);
          }
          return thresholdsVector;
        }

      private:
        size_t size;
        int maxValue;
        int minValue;
        static std::mt19937 mt_;
    };

  }
}

#endif /* THRESHOLDS_VECTOR_H */

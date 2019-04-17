#ifndef FEATURES_VECTOR_H
#define FEATURES_VECTOR_H


#include <oxylus/configuration/configuration_object.h>
#include <oxylus/memory/helper.h>
#include <oxylus/training/features.h>

namespace rdf{
  namespace bpc{

    class FeaturesVector{
      public:
        FeaturesVector();
        FeaturesVector(ConfigurationObject* configObject){
          this->minValue = configObject->GetFeaturesMinValue();
          this->maxValue = configObject->GetFeaturesMaxValue();
        }
        FeaturesVector(size_t size, int max, int min){
          this->size = size;
          this->maxValue = max;
          this->minValue = min;
        }

        std::vector<Features> GenerateVector(){
          std::uniform_int_distribution<int> features_dist(minValue, maxValue);
          std::vector<int> featuresVector;
          for (size_t i = 0; i < size; i++){
            Features features
            int value1 = features_dist(mt_);
            int value2 = features_dist(mt_);
            int value3 = features_dist(mt_);
            int value4 = features_dist(mt_);
            features.SetFeature1(value1, value2);
            features.SetFeature2(value3, value4);
            featuresVector.push_back(features);
          }
          return featuresVector;
        }
      private:
        size_t size;
        int maxValue;
        int minValue;
        static std::mt19937 mt_;
    };

    FeaturesVector::mt_ = Helper::InitializeRandomDevice();

  }
}

#endif /* FEATURES_VECTOR_H */

#ifndef FEATURES_VECTOR_H
#define FEATURES_VECTOR_H


#include <oxylus/memory/helper.h>
#include <oxylus/training/features.h>

namespace rdf{
  namespace bpc{
    class FeaturesVector{
      public:
        FeaturesVector();
        FeaturesVector(size_t size, float max, float min){
          this->size = size;
          this->maxValue = max;
          this->minValue = min;
        }

        std::vector<Features> GenerateVector(){
          std::uniform_real_distribution<float> features_dist(minValue, maxValue);
          std::vector<float> featuresVector;
          for (size_t i = 0; i < size; i++){
            Features features
            float value1 = features_dist(mt_);
            float value2 = features_dist(mt_);
            float value3 = features_dist(mt_);
            float value4 = features_dist(mt_);
            features.SetFeature1(value1, value2);
            features.SetFeature2(value3, value4);
            featuresVector.push_back(features);
          }
          return featuresVector;
        }
      private:
        size_t size;
        float maxValue;
        float minValue;
        static std::mt19937 mt_;
    };

    FeaturesVector::mt_ = Helper::InitializeRandomDevice();

  }
}

#endif /* FEATURES_VECTOR_H */

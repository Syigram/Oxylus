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
          float randomFeature1First;
          float randomFeature1Second;
          float randomFeature2First;
          float randomFeature2Second;

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

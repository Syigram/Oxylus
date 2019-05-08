#ifndef FEATURES_VECTOR_H
#define FEATURES_VECTOR_H

#include <random>
#include <iostream>

#include <oxylus/configuration/configuration_object.h>
#include <oxylus/memory/helper.h>
#include <oxylus/training/features.h>

namespace rdf{
  namespace bpc{

    class FeaturesVectorGenerator{
      public:
        FeaturesVectorGenerator();
        FeaturesVectorGenerator(ConfigurationObject* configObject){
          this->minValue = configObject->GetFeaturesMinValue();
          this->maxValue = configObject->GetFeaturesMaxValue();
          this->size = configObject->GetFeaturesSize();
          std::cout << "size from vectorGen" << size << std::endl;

        }
        FeaturesVectorGenerator(size_t size, int max, int min){
          this->size = size;
          this->maxValue = max;
          this->minValue = min;
        }

        /* std::vector<Features> GenerateVector(){ */
        /*   std::uniform_int_distribution<int> features_dist(minValue, maxValue); */
        /*   std::vector<Features> featuresVector; */
        /*   featuresVector.reserve(size); */
        /*   for (size_t i = 0; i < size; i++){ */
        /*     Features features; */
        /*     int value1 = features_dist(mt_); */
        /*     int value2 = features_dist(mt_); */
        /*     int value3 = features_dist(mt_); */
        /*     int value4 = features_dist(mt_); */
        /*     features.SetDelta1(value1, value2); */
        /*     features.SetDelta2(value3, value4); */
        /*     featuresVector.push_back(features); */
        /*   } */
        /*   return featuresVector; */
        /* } */
        std::vector<Features>* GenerateVector(){
          std::uniform_int_distribution<int> features_dist(minValue, maxValue);
          /* std::vector<Features>* featuresVector = new std::vector<Features>[size]; */
          std::vector<Features>* featuresVector = new std::vector<Features>();
          featuresVector->reserve(size);
          for (size_t i = 0; i < size; i++){
            Features features;
            int value1 = features_dist(mt_);
            int value2 = features_dist(mt_);
            int value3 = features_dist(mt_);
            int value4 = features_dist(mt_);
            features.SetDelta1(value1, value2);
            features.SetDelta2(value3, value4);
            featuresVector->push_back(features);
          }
          return featuresVector;
        }

        static std::mt19937 mt_;
      private:
        size_t size;
        int maxValue;
        int minValue;
    };

    std::mt19937 FeaturesVectorGenerator::mt_ = Helper::InitializeRandomDevice();

  }
}

#endif /* FEATURES_VECTOR_H */

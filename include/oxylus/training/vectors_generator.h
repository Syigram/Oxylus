#ifndef VECTORS_GENERATOR_H
#define VECTORS_GENERATOR_H

#include <random>

#include <oxylus/configuration/configuration_object.h>
#include <oxylus/training/features.h>

namespace rdf{
  namespace bpc{

    class VectorsGenerator{

      public:
        VectorsGenerator();
        VectorsGenerator(ConfigurationObject* configObject);
        std::vector<int>* GenerateThresholdsVector();
        std::vector<Features>* GenerateFeaturesVector();
        static std::mt19937 mt_;
        virtual ~VectorsGenerator(){};

      private:
        int minThresholdsValue;
        int maxThresholdsValue;
        int minFeaturesValue;
        int maxFeaturesValue;
        int featuresVectorSize;
        int thresholdsVectorSize;
        static std::mt19937 InitializeRandomDevice();
    };
  }
}


#endif /* VECTORS_GENERATOR_H */

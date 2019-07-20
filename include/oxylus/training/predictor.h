#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <string>
#include <vector>

#include <oxylus/configuration/configuration_object.h>
#include <oxylus/training/trainer.h>


namespace rdf {
  namespace bpc {
    class Predictor {
      public:
        Predictor();
        Predictor(ConfigurationObject* configObject);
        void DeserializeTrees();
        std::vector<int> Predict(cv::Mat_<ushort>& image, Node* node, int x, int y);
        void PredictBatchOfImages(int start, int end);
        void BeginPredictionForImage(std::string imagePath);
        virtual ~Predictor(){};
      private:
        cv::Vec3b GetRGBFromIndex(int index);
        void DeserializeTree(std::string path);
        std::vector<int> SumAllVectors(
            std::vector<int> v1,
            std::vector<int> v2,
            std::vector<int> v3
            );
        int HighestIndexFromHistograms(std::vector<int> histograms);
        ConfigurationObject* configurationObject;
        Tree tree1;
        Tree tree2;
        Tree tree3;

    };
  }
}

#endif /* PREDICTOR_H */

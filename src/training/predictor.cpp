#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <oxylus/training/predictor.h>
#include <oxylus/memory/file_reader.h>
#include <oxylus/memory/helper.h>

using namespace rdf::bpc;


Predictor::Predictor() {
  DeserializeTrees();

}

Predictor::Predictor(ConfigurationObject* configObject) {
  this->configurationObject = configObject;
  DeserializeTrees();
}

void Predictor::DeserializeTrees() {
  std::string path = this->configurationObject->GetOutputFolder();
  tree1.Deserialize(path + "/tree0.dat");
  tree2.Deserialize(path + "/tree1.dat");
  tree3.Deserialize(path + "/tree2.dat");
  std::cout << "Success deserializing"  << std::endl;
  
}

cv::Vec3b Predictor::GetRGBFromIndex(int index) {
  switch (index) {
    case 0:
      return cv::Vec3b(0, 0, 0);
    case 1:
      return cv::Vec3b(0, 0, 255);
    case 2:
      return cv::Vec3b(0, 255, 0);
    case 3:
      return cv::Vec3b(0, 255, 255);
    case 4:
      return cv::Vec3b(255, 0, 0);
    case 5:
      return cv::Vec3b(255, 0, 255);
    case 6:
      return cv::Vec3b(255, 255, 0);
    case 7:
      return cv::Vec3b(255, 255, 255);
    default:
      return cv::Vec3b(0, 0, 0);
      
  }

}

void Predictor::PredictBatchOfImages(int start, int end) {
  while (start <= end) {
    std::string filename = Helper::ImageFileNameHandler(configurationObject->GetFilePrefix(), start);
    std::string depthFilename = configurationObject->GetBaseFolder() +
      configurationObject->GetDepthFolder() + filename + configurationObject->GetFileExtension();
    BeginPredictionForImage(depthFilename);
    start++;
  }
}

void Predictor::BeginPredictionForImage(std::string imagePath) {
  FileReader fileReader;
  /* cv::Mat_<ushort> image = cv::imread(imagePath, CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR); */
  cv::Mat_<ushort> image = fileReader.ReadDepthImage(imagePath);
  int rows = image.rows;
  int cols = image.cols;
  cv::Mat predictedImage(rows, cols, CV_8UC3, cv::Scalar(0, 0, 0));
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int depthValue = (int) image.at<ushort>(i, j);
      if (depthValue != 0) {
        std::vector<int> h1 = Predict(image, tree1.GetRoot(), i, j);
        std::vector<int> h2 = Predict(image, tree2.GetRoot(), i, j);
        std::vector<int> h3 = Predict(image, tree3.GetRoot(), i, j);
        std::vector<int> histograms = SumAllVectors(h1, h2, h3);
        int index = HighestIndexFromHistograms(histograms);
        cv::Vec3b predictedColor = GetRGBFromIndex(index); 
        predictedImage.at<cv::Vec3b>(i, j) = predictedColor;
      } else {
        cv::Vec3b predictedColor = cv::Vec3b(0, 0, 0);
        predictedImage.at<cv::Vec3b>(i, j) = predictedColor;
      }
    }
  }
  cv::namedWindow( "Display window", cv::WINDOW_NORMAL );// Create a window for display.
  cv::resizeWindow( "Display window", 320, 240);
  cv::imshow( "Display window", predictedImage);                   // Show our image inside it.
  cv::waitKey(0);
}

int Predictor::HighestIndexFromHistograms(std::vector<int> histograms) {
  int max = 0;
  int maxIndex = 0;
  for (int i = 0; i < histograms.size(); ++i) {
    int current = histograms.at(i);
    if ( current > max) {
      max = current;
      maxIndex = i;
    }
  }
  return maxIndex;
}


std::vector<int> Predictor::SumAllVectors(
    std::vector<int> v1,
    std::vector<int> v2,
    std::vector<int> v3
    ) {
  for (int i = 0; i < v1.size(); ++i) {
    v1[i] += v2[i] + v3[i];    
  }
  return v1;
}

std::vector<int> Predictor::Predict(cv::Mat_<ushort>& image, Node* node, int x, int y) {
  if (dynamic_cast<WeakLearnerNode*>(node)) {
    WeakLearnerNode* weakNode = dynamic_cast<WeakLearnerNode*>(node);
    int id = weakNode->GetNodeId();
    Trainer trainer(this->configurationObject);
    Features features = weakNode->features;
    int threshold = weakNode->threshold;
    int z_u = (int) image.at<ushort>(x, y);
    cv::Point p1 = trainer.FeatureResponsePixel(features.GetDelta1(), x, y, z_u);
    cv::Point p2 = trainer.FeatureResponsePixel(features.GetDelta2(), x, y, z_u);
    int response = trainer.CalculateFeatureResponse(image, p1, p2);
    if (response >= threshold) {
      if (node->GetRight()){
        return this->Predict(image, node->GetRight(), x, y);
      } else {
        return this->Predict(image, node->GetLeft(), x, y);
      }
    } else {
      if (node->GetLeft()) {
        return this->Predict(image, node->GetLeft(), x, y);
      } else {
        return this->Predict(image, node->GetRight(), x, y);
      }
    }
  } else {
    LeafNode* leafNode = dynamic_cast<LeafNode*>(node);
    return leafNode->histogram;
  }
}


void Predictor::DeserializeTree(std::string path) {

}

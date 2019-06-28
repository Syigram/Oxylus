#include <oxylus/training/trainer.h>
#include <oxylus/configuration/configuration_constants.h>


using namespace rdf::bpc;


Trainer::Trainer(){

}

Trainer::Trainer(ConfigurationObject* configurationObject){
  this->configurationObject = configurationObject;
  this->highestDepthValue = rdf::bpc::constants::HIGH_VALUE;
  this->highIntValue = rdf::bpc::constants::HIGH_VALUE;
  this->maxCols = configurationObject->GetImagesCols();
  this->maxRows = configurationObject->GetImagesRows();
  this->featuresSize = configurationObject->GetFeaturesSize();
  this->thresholdsSize = configurationObject->GetThresholdsSize();
}

void Trainer::TrainNode(std::shared_ptr<ImagesVector> imagesVec, NodeVectors& nodeVectors, int treeId){
  int featuresSize = configurationObject->GetFeaturesSize();
  int thresholdsSize = configurationObject->GetThresholdsSize();
  int trainingNodeId = nodeVectors.nodeId;
  Cell myCell;
  /* Matrix<Cell> nodeHistograms(featuresSize, thresholdsSize, Cell()); */
  Matrix<Cell> nodeHistograms(featuresSize, thresholdsSize, myCell);
  int featuresIndex = 0;
  int thresholdIndex = 0;
  for (auto& imageStructure: *imagesVec) {
    if (imageStructure.treesId.at(treeId) == 1) {
      for (auto& pointStructure: *(imageStructure.pointsVector)) {
        if (trainingNodeId == pointStructure.GetCurrentNode()){
          cv::Mat_<ushort>& depthImage = imageStructure.depthImage;
          int z_u = (int) depthImage.at<ushort>(pointStructure.GetPoint());
          featuresIndex = 0;
          for (auto& features: *(nodeVectors.featuresVec)) {
            cv::Point point1 = CalculateFeatureResponsePoint(features.GetDelta1(), pointStructure, z_u);
            cv::Point point2 = CalculateFeatureResponsePoint(features.GetDelta2(), pointStructure, z_u);
            int featureResponse = CalculateFeatureResponse(depthImage, point1, point2);
            thresholdIndex = 0;
            for (auto thresholdN: *(nodeVectors.thresholdsVec)) {
              Cell& cell = nodeHistograms[featuresIndex][thresholdIndex];
              int labelValue = pointStructure.GetLabelPixelValue();
              if (featureResponse >= thresholdN){
                cell.rightHistogram[labelValue]++;
              } else {
                cell.leftHistogram[labelValue]++;
              }
              thresholdIndex++;
            }
            featuresIndex++;
          }
        }
      }
    }
  }
  nodeVectors.nodeHistograms = nodeHistograms;
}


int Trainer::CalculateFeatureResponse(cv::Mat_<ushort>& depthImage, cv::Point point1, cv::Point point2){
  int depthValue1 = CalculateDepthValue(depthImage, point1);
  int depthValue2 = CalculateDepthValue(depthImage, point2);
  std::uniform_int_distribution<int> index_dist(0, 1);
  /* int randomProbability = index_dist(Trainer::mt_); */
  /* int highValue = configurationObject->GetHighestDepthValue();; */
  /* std::cout << "depthResponse" << depthValue1 - depthValue2 << std::endl; */
  return depthValue1 - depthValue2;
}

int Trainer::CalculateDepthValue(cv::Mat_<ushort>& depthImage, cv::Point point) {
  int depthValue = 0;
  if (point.y < maxRows && point.x < maxCols && point.y >= 0 && point.x >= 0){
    depthValue = (int) depthImage.at<ushort>(point);
  }
  else {
    depthValue = highestDepthValue;
  }
  return depthValue;
}


cv::Point Trainer::CalculateFeatureResponsePoint(cv::Point point, 
                                  PointStructure& pointStruct, int z_u){
  int uX = pointStruct.GetX();
  int uY = pointStruct.GetY();
  int deltaX = point.x;
  int deltaY = point.y;
  float deltaXNormalized = (float) deltaX / (float) z_u;
  float deltaYNormalized = (float) deltaY / (float) z_u;
  /* std::cout << "offsetX: " << deltaXNormalized << std::endl; */
  /* std::cout << "offsetY: " << deltaYNormalized << std::endl; */
  /* if (deltaXNormalized > 1000 || deltaXNormalized < -1000) { */
  /*   std::cout << "x: " << uX << std::endl; */
  /* } */
  /* if (deltaYNormalized > 1000 || deltaYNormalized < -1000) { */
  /*   std::cout << "y: " << uY << std::endl; */
  /* } */
  int pointX = uX + (int) deltaXNormalized;
  int pointY = uY + (int) deltaYNormalized;
  cv::Point myPoint(pointX, pointY);
  return myPoint;
}


WeakLearnerNode* Trainer::CreateTrainedNode(
    int nodeId,
    int bestFeatureIndex,
    int bestThresholdIndex,
    NodeVectors& nodeVectors) {
  Features bestFeatures = nodeVectors.featuresVec->at(bestFeatureIndex);
  int bestThreshold = nodeVectors.thresholdsVec->at(bestThresholdIndex);
  WeakLearnerNode* node  = new WeakLearnerNode(nodeId, bestFeatures, bestThreshold);
  return node;

}


LeafNode* Trainer::CreateLeafNode(int nodeId, std::shared_ptr<ImagesVector> imagesVec) {
  LeafNode* node = new LeafNode(nodeId);
  for (auto& image: *imagesVec) {
    for (auto& point: *(image.pointsVector)) {
      if (point.GetCurrentNode() == nodeId){
        int labelValue = point.GetLabelPixelValue();
        node->histogram[labelValue]++;
      }
    }
  }
  return node;
}


std::vector<int> Trainer::CreateHistogramForLeafNode(int nodeId, std::shared_ptr<ImagesVector> imagesVec) {


}


void Trainer::EvaluateImages(std::shared_ptr<ImagesVector> imagesVec, WeakLearnerNode* weakNode) {
  int nodeId = weakNode->nodeId;
  int newNodeId = 0;
  int rightNodeId = (nodeId * 2) + 1;
  int leftNodeId = nodeId * 2;
  int threshold = weakNode->threshold;
  int leftCounter = 0;
  int rightCounter = 0;
  for (auto& imageStructure: *imagesVec) {
    for (auto& pointStructure: *(imageStructure.pointsVector)) {
      if (nodeId == pointStructure.GetCurrentNode()){
        cv::Mat_<ushort>& depthImage = imageStructure.depthImage;
        int z_u = (int) depthImage.at<ushort>(pointStructure.GetPoint());
        cv::Point point1 = CalculateFeatureResponsePoint(weakNode->features.GetDelta1(), pointStructure, z_u);
        cv::Point point2 = CalculateFeatureResponsePoint(weakNode->features.GetDelta2(), pointStructure, z_u);
        int featureResponse = CalculateFeatureResponse(depthImage, point1, point2);
        if (featureResponse >= threshold) {
          newNodeId = rightNodeId;
          rightCounter++;
        } else {
          newNodeId = leftNodeId;
          leftCounter++;
        }
        pointStructure.SetCurrentNode(newNodeId);
      }
    }
  }
}


std::mt19937 Trainer::InitRandomSeed() {
    std::random_device rd;
    std::mt19937 mt(rd());
    return mt;
}



std::mt19937 Trainer::mt_ = Trainer::InitRandomSeed();


rdf::Matrix<Cell> Trainer::ReduceHistograms(std::vector<NodeVectors> &gatheredNodeVectors) {
  Cell myCell;
  Matrix<Cell> histogramsAccumulated(featuresSize, thresholdsSize, myCell);
  for (auto& nodeVector: gatheredNodeVectors) {
    for (int i = 0; i < featuresSize; i++) {
      for (int j = 0; j < thresholdsSize; j++) {
        histogramsAccumulated[i][j].Reduce(nodeVector.nodeHistograms[i][j]);
      }
    }
  }
  return histogramsAccumulated;
}


void Trainer::UpdateHistogramsCount(Matrix<Cell>& nodeHistograms) {
  for (int i = 0; i < featuresSize; i++) {
    for (int j = 0; j < thresholdsSize; j++) {
      nodeHistograms[i][j].SetHistogramsTotalCount();
    }
  }
}


double Trainer::GetArgMinValue(Cell& cell) {
  double S_L_Magnitude = (double) cell.leftHistogramTotal;
  double S_R_Magnitude = (double) cell.rightHistogramTotal;
  double S_Magnitude = (double) cell.totalCount;
  /* std::cout << "s_l=" << S_L_Magnitude << "\ts_r=" << S_R_Magnitude << "\ts_m=" << S_Magnitude << std::endl; */
  double I_L = ClassificationObjectiveFunction(cell.leftHistogram, S_L_Magnitude);
  double I_R = ClassificationObjectiveFunction(cell.rightHistogram, S_R_Magnitude);
  double SL_BY_IL = ArgMinFunction(I_L, S_L_Magnitude, S_Magnitude);
  double SR_BY_IR = ArgMinFunction(I_R, S_R_Magnitude, S_Magnitude);
  /* std::cout << "IL=" << I_L << "\tIR="<< I_R << std::endl; */
  return SL_BY_IL + SR_BY_IR;
  /* std::cout << "argMin value: " << cell.argMinValue << std::endl; */
}

double Trainer::ArgMinFunction(
    double classification_objective,
    double side_magnitude,
    double total_magnitude) {
  if (classification_objective == this->highIntValue) {
    return highIntValue;
  } else if (classification_objective >= 0.0) {
    return (side_magnitude / total_magnitude) * classification_objective;
  } else {
    std::cerr << "Error: Classification objective result has a negative value" 
              << classification_objective << std::endl;
    return 0.0;
  }
}

double Trainer::ClassificationObjectiveFunction(std::vector<int> histogram,
                                                double histogram_magnitude) {
  double result = 0.0;
  int totalBodyParts = constants::BODY_PARTS;
  for (int i = 0; i < totalBodyParts; i++) {
    double P_C_S = (double) histogram[i] / (double) histogram_magnitude;
    if (std::isnan(P_C_S)){
      return this->highIntValue;
    } else if (P_C_S > 0.0) {
      double log_P_C_S = log(P_C_S);
      result += (P_C_S * log_P_C_S);
    } else {
      result += 0;
    }
  }
  return GetPositiveResult(result);
}
  
double Trainer::GetPositiveResult(double result) {
  if (result == 0.0) {
    return 0.0;
  } else {
    return result * -1;
  }
}

std::pair<int, int> Trainer::FindLowestArgMin(Matrix<Cell>& nodeHistograms) {
  double lowestArgMin = this->highIntValue;
  int featuresIndex = 0;
  int thresholdIndex = 0;
  for (int i = 0; i < featuresSize; i++) {
    for (int j = 0; j < thresholdsSize; j++) {
      Cell& currentCell = nodeHistograms[i][j];
      double argMinVal = GetArgMinValue(currentCell);
      if (argMinVal < lowestArgMin) {
        lowestArgMin = argMinVal;
        currentCell.argMinValue = argMinVal;
        featuresIndex = i;
        thresholdIndex = j;
      } else {
      }
    }
  }
  /* std::cout << "lowestArgMin: " << lowestArgMin << std::endl; */
  /* std::cout << "featIndex: " << featuresIndex << "\tthreshIndex: "<< thresholdIndex << std::endl; */
  /* std::cout << "left count: " << nodeHistograms[featuresIndex][thresholdIndex].leftHistogramTotal; */
  /* std::cout << "right count: " << nodeHistograms[featuresIndex][thresholdIndex].rightHistogramTotal; */
  return std::make_pair(featuresIndex, thresholdIndex);
}


int Trainer::NodeHasMinimunPoints(std::vector<int> pointsCount) {
  int minPointsPerNode = this->configurationObject->GetStopCondition();
  for (auto count: pointsCount) {
    if (count < minPointsPerNode) {
      return 0;
    }
  }
  return 1;
}


LeafNode* Trainer::CreateLeafNodeFromParent(int nodeId, std::vector<int> histograms) {
  LeafNode* node = new LeafNode(nodeId, histograms);
  return node;
}

void Trainer::CheckForLeafNodes(
    int parentNodeId, 
    Cell& bestCell,
    Tree& tree, 
    std::vector<int>& leafNodesList) {
  int leftCount = bestCell.leftHistogramTotal;
  int rightCount = bestCell.rightHistogramTotal;
  if (!HasMinimunPoints(leftCount)) {
    int leftNodeId = parentNodeId * 2;
    LeafNode* leftNode = CreateLeafNodeFromParent(leftNodeId, bestCell.leftHistogram);       
    tree.Insert(leftNode);
    leafNodesList.push_back(leftNodeId);
  }
  if (!HasMinimunPoints(rightCount)) {
    int rightNodeId = (parentNodeId * 2) + 1;
    LeafNode* rightNode = CreateLeafNodeFromParent(rightNodeId, bestCell.rightHistogram);       
    tree.Insert(rightNode);
    leafNodesList.push_back(rightNodeId);
  }
}

bool Trainer::HasMinimunPoints(int count) {
  int stopCondition = this->configurationObject->GetStopCondition();
  if (count >= stopCondition) return true;
  else return false;
}


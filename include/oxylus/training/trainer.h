#ifndef TRAINER_H
#define TRAINER_H

#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>

#include <oxylus/training/matrix.h>
#include <oxylus/training/cell.h>
#include <oxylus/configuration/configuration_object.h>
#include <oxylus/image/image_operations.h>
#include <oxylus/training/node_vectors.h>
#include <oxylus/training/weak_learner_node.h>
#include <oxylus/training/leaf_node.h>
#include <oxylus/training/tree.h>


namespace rdf {
  namespace bpc {

    class Trainer {
      public:
        Trainer();
        Trainer(ConfigurationObject* configurationObject);
        /* PartialTrainedNode TrainNode(std::shared_ptr<ImagesVector> imagesVec, NodeVectors& nodeVectors); */
        void TrainNode(std::shared_ptr<ImagesVector> imagesVec, 
                       NodeVectors& nodeVectors, int treeId);
        WeakLearnerNode* CreateTrainedNode(int nodeId, int bestFeatureIndex,
                                           int bestThresholdIndex, 
                                           NodeVectors& nodeVectors);
        LeafNode* CreateLeafNode(int nodeId, 
                                 std::shared_ptr<ImagesVector> imagesVec);
        LeafNode* CreateLeafNodeFromParent(int nodeId, 
                                           std::vector<int> histograms);
        void EvaluateImages(std::shared_ptr<ImagesVector> imagesVec, WeakLearnerNode* weakNode);
        void CheckForLeafNodes(int parentNodeId, Cell& bestCell, Tree& tree,
            std::vector<int> leafNodesList);
        Matrix<Cell> ReduceHistograms(std::vector<NodeVectors> &gatheredNodeVectors);
        void UpdateHistogramsCount(Matrix<Cell>& nodeHistograms);
        int NodeHasMinimunPoints(std::vector<int> pointsCount);
        std::pair<int, int> FindLowestArgMin(Matrix<Cell>& nodeHistograms);
        ~Trainer(){};

      private:
        int CalculateFeatureResponse(cv::Mat_<ushort>& depthImage,
                                     cv::Point point1, cv::Point point2);
        cv::Point CalculateFeatureResponsePoint(cv::Point point,
                                                PointStructure& pointStruct,
                                                int z_u);
        int CalculateDepthValue(cv::Mat_<ushort>& depthImage, cv::Point point);


        double ClassificationObjectiveFunction(std::vector<int> histogram,
                                               double magnitude);
        double GetArgMinValue(Cell& cell);
        double ArgMinFunction(double classification_objective, 
                              double side_magnitude, 
                              double total_magnitude);

        std::vector<int> CreateHistogramForLeafNode(
            int nodeId,
            std::shared_ptr<ImagesVector> imagesVec);


        bool HasMinimunPoints(int count);

        double GetPositiveResult(double result);


        ConfigurationObject* configurationObject;
        int highestDepthValue;
        int highIntValue;

        int maxRows;
        int maxCols;

        int featuresSize;
        int thresholdsSize;

        //<! MersenneTwister pseudo-random engine
        static std::mt19937 mt_;
        //<! Static function for initializing seed for random distribution.
        //<! It is only initialized once.
        static std::mt19937 InitRandomSeed();

    };
  }
}


#endif /* TRAINER_H */

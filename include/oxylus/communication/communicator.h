#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <string>
#include <memory>

#include <boost/mpi.hpp>

#include <oxylus/configuration/configuration_constants.h>
#include <oxylus/configuration/configuration_object.h>
#include <oxylus/mpi_message/memory_message.h>
#include <oxylus/mpi_message/image_allocation_message.h>
#include <oxylus/image/image_structure.h>
#include <oxylus/training/node_vectors.h>

#define MPI_MASTER 0

namespace rdf{
  namespace bpc{

    typedef std::vector<MemoryMessage> MemoryMessageVec;
    typedef std::vector<ImageBatchMessage> ImageBatchMessageVec;


    namespace mpi = boost::mpi;

    class Communicator{
      public:
        Communicator();
        Communicator(ConfigurationObject* configObject);
        void AssignImagesToEachNode();
        void LoadImagesToStructures();
        void AssignTreeBitsToImages(std::shared_ptr<ImagesVector> imagesVec);
        void BeginTraining();
        void ExecutePointsSelection();
        void StartDistributedTraining();
        int GetRank();
        virtual ~Communicator(){
        }

      private:
        ConfigurationObject* configObject;
        mpi::environment environment;
        mpi::communicator world;
        MemoryMessageVec memoryMessageVec;
        ImageBatchMessage imageBatchMessage;
        std::shared_ptr<ImagesVector> imagesStructureVector;
        size_t size;
        int rank;
        int master;
        int featuresSize;
        int thresholdsSize;
        void GatherMemoryInformationMessage();
        int NodeHasMinimunPoints(std::vector<int> pointsCount);
        void ScatterImagesBatchMessage(ImageBatchMessage imageBatchMessage);
        void MasterPrint(std::string message);
        Matrix<Cell> ReduceHistograms(std::vector<NodeVectors> &gatheredNodeVectors);
        void UpdateHistogramsCount(Matrix<Cell>& nodeHistograms);
        NodeVectors BroadcastNodeVectors(int nodeId);
        double ClassificationObjectiveFunction(std::vector<int> histogram, double magnitude);
        double GetArgMinValue(Cell& cell);
        std::pair<int, int> FindLowestArgMin(Matrix<Cell>& nodeHistograms);

        bool CheckValidNodeVectors(std::vector<NodeVectors> &gatheredNodeVectors);
        /* void LookForLeafNodes(); */
        /* void CountHistograms(Matrix<Cell>& nodeHistograms, int bestFeatureIndex, */
        /*     int bestThresholdIndex); */
    };
  }
}


#endif /* COMMUNICATOR_H */

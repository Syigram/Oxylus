#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <string>
#include <memory>

#include <boost/mpi.hpp>

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
        virtual ~Communicator(){};

      private:
        mpi::environment environment;
        mpi::communicator world;
        MemoryMessageVec memoryMessageVec;
        ImageBatchMessage imageBatchMessage;
        std::shared_ptr<ImagesVector> imagesStructureVector;
        size_t size;
        int rank;
        int master;
        ConfigurationObject* configObject;
        void GatherMemoryInformationMessage();
        void ScatterImagesBatchMessage(ImageBatchMessage imageBatchMessage);
        void MasterPrint(std::string message);
        NodeVectors BroadcastNodeVectors(int nodeId);

    };
  }
}


#endif /* COMMUNICATOR_H */

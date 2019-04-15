#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <boost/mpi.hpp>

#include <oxylus/configuration/configuration_object.h>

#define MPI_MASTER 0

namespace rdf{
  namespace bpc{


    namespace mpi = boost::mpi;

    class Communicator{
      public:
        Communicator();
        Communicator(ConfigurationObject* configObject);
        void AssignImagesToEachNode();
        void ExecutePointsSelection();
        void StartDistributedTraining();
        virtual ~Communicator(){};

      private:
        mpi::environment environment;
        mpi::communicator world;
        size_t size;
        int rank;
        int master;
        ConfigurationObject* configObject;

    };
  }
}


#endif /* COMMUNICATOR_H */

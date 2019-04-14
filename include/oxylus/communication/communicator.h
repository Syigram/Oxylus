#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <boost/mpi.hpp>


namespace rdf{
  namespace bpc{

    namespace mpi = boost::mpi;

    class Communicator{
      public:
        Communicator();
        virtual ~Communicator(){};

      private:
        mpi::environment environment;
        mpi::communicator world;
    }
  }
}


#endif /* COMMUNICATOR_H */

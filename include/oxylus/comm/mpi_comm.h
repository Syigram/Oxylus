#include <boost/mpi.hpp>

namespace mpi = boost::mpi;

class MPIComm {
private:
  /* data */
  mpi::environment environment;
  mpi::communicator world;

public:
  MPIComm ();
  void SendInitialMessage(std::string message);
  void ReceiveInitalMessage(std::string message);
  virtual ~MPIComm ();
};

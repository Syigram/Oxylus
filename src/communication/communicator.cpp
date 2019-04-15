#include <oxylus/communication/communicator.h>

#include <oxylus/mpi_message/memory_message.h>


using namespace rdf::bpc;

typedef std::vector<MemoryMessage> MemoryMessageVec;

Communicator::Communicator(){
  this->rank = world.rank();
  this->size = world.size();
}

Communicator::Communicator(ConfigurationObject* configObject){
  this->rank = world.rank();
  this->size = world.size();
  this->configObject = configObject;
}

void Communicator::AssignImagesToEachNode(){
  MemoryMessageVec memoryMessageVec;
  MemoryMessage memoryMessage(rank);
  memoryMessage.CreateMemoryMessage();
  memoryMessage.Print();
  mpi::gather(world, memoryMessage, memoryMessageVec, MPI_MASTER);

  /* mpi::gather */

}


void Communicator::ExecutePointsSelection(){

}


void Communicator::StartDistributedTraining(){

}

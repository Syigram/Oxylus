#include <oxylus/memory/mem_info.h>
#include <oxylus/memory/image_allocator.h>
#include <oxylus/comm/mpi_memory_message.h>

#include <string>
#include <boost/mpi.hpp>
#include <iostream>
#include <vector>


namespace mpi = boost::mpi;



int main(int argc, char const *argv[]) {
  boost::mpi::environment env;
  boost::mpi::communicator world;
  int rank = world.rank();
  int worldSize = world.size();
  if (rank == 0){
    std::cout << "Retrieving available memory from all nodes in the cluster..." << '\n';
  }
  else{
    std::cout << "Somos esclavos, lel lel" << '\n';
  }

  MemInfo memInfo;
  memInfo.SetMemoryInformation(rank);
  MPIMemoryMessage message = memInfo.GetAllMemoryInformation();
  message.SetIdProcess(rank);
  std::cout << "here" << message.GetMemTotal() << '\n';
  message.Print();
  // std::cout << "here2" << '\n';
  if (rank == 0){
    std::vector<MPIMemoryMessage> memoryMessageVector;
    memoryMessageVector.resize(worldSize);
    mpi::gather<MPIMemoryMessage>(world, message, memoryMessageVector, 0);
    int i = 0;
    for (MPIMemoryMessage mess : memoryMessageVector){
      std::cout << "Printing for process: " << i++ << '\n';
      mess.Print();
    }
  } else{
    std::cout << "Slave #" << rank << "sending memory message..." << '\n';
    mpi::gather(world, message, 0);
  }

  // ImageAllocator alloc;
  // MemInfo
  // int total = alloc.GetMaxBatchSize();

  // std::string val = memInfo.GetMemoryValue(TOTAL);
  // std::string val = exec("cat /proc/meminfo | grep MemAvailable");
  // std::cout << "values: " << val  << '\n';
  return 0;
}

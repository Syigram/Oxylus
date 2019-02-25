#include <oxylus/memory/mem_info.h>
#include <oxylus/memory/image_allocation_message.h>
#include <oxylus/mpi_message/memory_message.h>
#include <oxylus/configuration/config.h>


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

  MemInfo memInfo;
  memInfo.SetMemoryInformation(rank);
  MemoryMessage message = memInfo.GetAllMemoryInformation();
  message.SetIdProcess(rank);
  if (rank == 0){
    std::vector<MemoryMessage> memoryMessageVector;
    memoryMessageVector.resize(worldSize);
    mpi::gather<MemoryMessage>(world, message, memoryMessageVector, 0);
    int i = 0;
    for (MemoryMessage mess : memoryMessageVector){
      std::cout << "Printing for process: " << i++ << '\n';
      int batchSize = mess.GetMemAvailable() / IMAGE_AVG_SIZE;
      std::cout << "BatchSize = " << batchSize << '\n';
      mess.Print();
    }
  } else{
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

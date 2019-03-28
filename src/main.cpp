#include <boost/mpi.hpp>
#include <boost/mpi/datatype.hpp>
#include <oxylus/memory/mem_info.h>
#include <oxylus/mpi_message/image_allocation_message.h>
#include <oxylus/mpi_message/memory_message.h>
#include <oxylus/configuration/config.h>

#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>

#define MPI_MASTER 0

namespace mpi = boost::mpi;

typedef std::vector<ImageAllocationMessage> ImageAllocVect;
typedef std::vector<MemoryMessage> MemAllocVect;



ImageAllocVect ExtractMemoryMessageInfo(MemAllocVect messages){
  ImageAllocVect imageAllocVector;
  MemAllocVect::iterator it;
  int counter = 0;
  int i = 0;
  for(it = messages.begin(); it != messages.end(); it++, i++){
    ImageAllocationMessage imageAllocMsg;
    int batchSize = it->GetMemAvailable() / IMAGE_AVG_SIZE;
    int start = counter;
    int end = counter + batchSize;
    imageAllocMsg.SetBatchSize(batchSize);
    imageAllocMsg.SetIndexStart(start);
    imageAllocMsg.SetIndexEnd(end);
    imageAllocMsg.SetIdProcess(i);
    counter = end + 1;
    imageAllocMsg.Print();
    imageAllocVector.push_back(imageAllocMsg);
  }
  return imageAllocVector;
}

int main(int argc, char const *argv[]) {
  mpi::environment env;
  mpi::communicator world;

  int rank = world.rank();
  int worldSize = world.size();

  MemoryMessage memMessage(rank);
  memMessage.Print();
  std::vector<MemoryMessage> memoryMessageVector;
  ImageAllocationMessage imageAllocScatter;

  mpi::gather<MemoryMessage>(world, memMessage, memoryMessageVector, 0);

  world.barrier();
//  if (rank == MPI_MASTER)
  ImageAllocVect imageAllocVector = ExtractMemoryMessageInfo(memoryMessageVector);

  mpi::scatter(world, imageAllocVector, imageAllocScatter, 0);
  std::cout << "---------------- Scatter begin ----------------" << '\n';
  imageAllocScatter.Print();


  // std::cout << "END: " <<  rank << '\n';
  // ImageAllocator alloc;
  // MemInfo
  // int total = alloc.GetMaxBatchSize();

  // std::string val = memInfo.GetMemoryValue(TOTAL);
  // std::string val = exec("cat /proc/meminfo | grep MemAvailable");
  // std::cout << "values: " << val  << '\n';
  return 0;
}

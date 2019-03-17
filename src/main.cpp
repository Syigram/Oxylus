#include <boost/mpi.hpp>
#include <oxylus/memory/mem_info.h>
#include <oxylus/mpi_message/image_allocation_message.h>
#include <oxylus/mpi_message/memory_message.h>
#include <oxylus/configuration/config.h>


#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>

BOOST_IS_MPI_DATATYPE(ImageAllocationMessage)
namespace mpi = boost::mpi;

typedef std::vector<ImageAllocationMessage> ImageAllocVect;
typedef std::vector<MemoryMessage> MemAllocVect;

ImageAllocVect ExtractMemoryMessageInfo(MemAllocVect messages, int size){
  ImageAllocVect imageAllocVector;
  imageAllocVector.resize(size);
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
    imageAllocMsg.SetIdProcess(i++);
    counter = end + 1;
    imageAllocMsg.Print();
    imageAllocVector.push_back(imageAllocMsg);
  }
  return imageAllocVector;
}

int main(int argc, char const *argv[]) {
  boost::mpi::environment env;
  boost::mpi::communicator world;

  int rank = world.rank();
  int worldSize = world.size();

  MemInfo memInfo;
  memInfo.SetMemoryInformation(rank);
  MemoryMessage message = memInfo.GetAllMemoryInformation();
  message.SetIdProcess(rank);
  ImageAllocVect imageAllocVector;
  std::vector<MemoryMessage> memoryMessageVector;
  imageAllocVector.resize(worldSize);
  if (rank == 0){
    memoryMessageVector.resize(worldSize);
    mpi::gather<MemoryMessage>(world, message, memoryMessageVector, 0);
    imageAllocVector = ExtractMemoryMessageInfo(memoryMessageVector, worldSize);
    std::cout << "printing from root" << '\n';
  } else{
    mpi::gather(world, message, 0);
  }
  world.barrier();
  ImageAllocationMessage imageAllocScatter;
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

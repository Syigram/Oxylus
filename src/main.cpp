// #include "mem_info.h"
#include <oxylus/memory/image_allocator.h>

#include <string>
#include <boost/mpi.hpp>
#include <iostream>


namespace mpi = boost::mpi;

int main(int argc, char const *argv[]) {
  // ImageAllocator alloc;
  // int total = alloc.GetMaxBatchSize();
  boost::mpi::environment env;
  boost::mpi::communicator world;
  std::cout << world.rank() << ", " << world.size() << '\n';
  // MemInfo memInfo;
  // memInfo.SetMemoryInformation();
  // std::string val = memInfo.GetMemoryValue(TOTAL);
  // std::string val = exec("cat /proc/meminfo | grep MemAvailable");
  // std::cout << "values: " << val  << '\n';
  return 0;
}

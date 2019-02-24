// #include "mem_info.h"
#include "image_allocator.h"
#include <string>
#include <iostream>

int main(int argc, char const *argv[]) {

  ImageAllocator alloc;
  int total = alloc.GetMaxBatchSize();

  // MemInfo memInfo;
  // memInfo.SetMemoryInformation();
  // std::string val = memInfo.GetMemoryValue(TOTAL);
  // std::string val = exec("cat /proc/meminfo | grep MemAvailable");
  // std::cout << "values: " << val  << '\n';
  return 0;
}

#include "mem_info.h"
#include <string>
#include <stdio.h>

int main(int argc, char const *argv[]) {

  MemInfo memInfo;
  memInfo.SetMemoryInformation();
  // std::string val = memInfo.GetMemoryValue(TOTAL);
  // std::string val = exec("cat /proc/meminfo | grep MemAvailable");
  // std::cout << "values: " << val  << '\n';
  return 0;
}

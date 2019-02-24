#include "helper.h"
#include "mem_info.h"

#include <iostream>
#include <regex>

void MemInfo::SetMemoryInformation(){
  std::cout << "[INFO] | Retrieving memory information...." << '\n';
  this->_memTotal = GetMemoryValue(TOTAL);
  this->_memAvailable = GetMemoryValue(AVAILABLE);
  this->_memFree = GetMemoryValue(FREE);
  std::cout << "[SUCCESS] | Memory information set!" << '\n';
}

int MemInfo::GetMemoryValue(MemType type){
  Helper helper;
  std::string shell_out= "";
  std::string mem_type_str = "";
  switch (type) {
    case TOTAL:
        shell_out = helper.run_shell_cmd("cat /proc/meminfo | grep MemTotal");
        mem_type_str = "Total";
        break;
    case AVAILABLE:
        shell_out = helper.run_shell_cmd("cat /proc/meminfo | grep MemAvailable");
        mem_type_str = "Available";
        break;
    case FREE:
        shell_out = helper.run_shell_cmd("cat /proc/meminfo | grep MemFree");
        mem_type_str = "Free";
        break;
  }
  int result = helper.extract_integer_from_str(shell_out);
  std::cout << "[INFO] | Memory" << mem_type_str << ": " << result << " kB" << '\n';
  return result;

}


int MemInfo::GetMemTotal(){
  return this->_memTotal;
}

int MemInfo::GetMemAvailable(){
  return this->_memAvailable;

}

int MemInfo::GetMemFree(){
  return this->_memFree;
}

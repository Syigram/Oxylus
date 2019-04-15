#include <iostream>
#include <regex>

#include <oxylus/memory/helper.h>
#include <oxylus/memory/mem_info.h>

#include <oxylus/image/image_structure.h>
#include <oxylus/image/point_structure.h>

using namespace rdf::bpc;
using namespace std;

MemInfo::MemInfo(){
}

MemInfo::MemInfo(ConfigurationObject* configObject){
  this->configObject = configObject;
  this->memoryUsage = configObject->GetMemoryUsage();
  this->LoadMemoryInformation();
}

void MemInfo::LoadMemoryInformation(){
  this->SetMemoryInformation();
  this->CalcBatchSize();
}

// void MemInfo::SetMemoryInformation(){
void MemInfo::SetMemoryInformation(){
  std::cout << "[INFO] | Retrieving memory information...." << '\n';
  this->memTotal = GetMemoryValue(TOTAL);
  this->memAvailable = GetMemoryValue(AVAILABLE);
  this->memFree = GetMemoryValue(FREE);
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
  return this->memTotal;
}

int MemInfo::GetMemAvailable(){
  return this->memAvailable;
}

int MemInfo::GetMemFree(){
  return this->memFree;
}

int MemInfo::GetBatchSize(){
  return this->batchSize;
}

int MemInfo::CalcBatchSize(){
  ImageStructure imageStructure(this->configObject);
  int imageStructureBytes = imageStructure.GetSizeOf(this->configObject);
  int memoryAvailable = GetMemAvailable();
  double memoryPercent = (double) this->memoryUsage / 100;
  double memoryUsable = (double) (memoryAvailable * memoryPercent);
  int estimatedBatchSize = (int) (memoryUsable / imageStructureBytes);
  this->batchSize = estimatedBatchSize;
  cout << "memoryUsage: " << memoryUsage << endl;
  cout << "memoryAvailable: " << memoryAvailable << endl;
  cout << "imageStructureBytes: " << imageStructureBytes << endl;
  cout << "memoryPercent: " << memoryPercent << endl;
  cout << "memoryUsable: " << memoryUsable << endl;
  cout << "estimatedBatchSize: " << estimatedBatchSize << endl;
  return this->batchSize;
}

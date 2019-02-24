
#include "image_allocator.h"
#include "mem_info.h"
#include <iostream>

int ImageAllocator::CalcBatchSize(){
  MemInfo memInfo;
  memInfo.SetMemoryInformation();
  int memAvailable = memInfo.GetMemAvailable();
  int totalImages = memAvailable / IMAGE_AVG_SIZE;
  std::cout << "Creo que puedo recibir: " << totalImages << " imagenes.\n";
  return totalImages;
}

int ImageAllocator::GetMaxBatchSize(){
  return CalcBatchSize();
}

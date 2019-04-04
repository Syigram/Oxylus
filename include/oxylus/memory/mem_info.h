#ifndef MEM_INFO_H

/* #include <oxylus/mpi_message/memory_message.h> */

#include <string>

enum MemType{
  TOTAL = 1,
  AVAILABLE = 2,
  FREE = 3
};

class MemInfo {
private:
  /* data */
  int _idProcess;
  int _memTotal;
  int _memAvailable;
  int _memFree;


public:
  int GetMemoryValue(MemType type);
  void SetMemoryInformation();
  /* MemoryMessage GetAllMemoryInformation(); */
  int GetMemTotal();
  int GetMemAvailable();
  int GetMemFree();
  MemInfo ();
  // virtual ~MemInfo ();
};




#define MEM_INFO_H
#endif

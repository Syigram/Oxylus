#ifndef MEM_INFO_H
#define MEM_INFO_H

#include <string>

#include <oxylus/configuration/configuration_object.h>

enum MemType{
  TOTAL = 1,
  AVAILABLE = 2,
  FREE = 3
};

namespace rdf {
  namespace bpc {


    class MemInfo {
    private:
      /* data */
      int idProcess;
      int memTotal;
      int memAvailable;
      int memFree;
      int batchSize;
      int memoryUsage;
      ConfigurationObject* configObject;
      int CalcBatchSize();
      int GetMemoryValue(MemType type);
      void SetMemoryInformation();
    public:
      MemInfo ();
      MemInfo(ConfigurationObject* configObject);
      void LoadMemoryInformation();
      int GetMemTotal();
      int GetMemAvailable();
      int GetMemFree();
      int GetBatchSize();
      virtual ~MemInfo (){};
    };
  }
}

#endif

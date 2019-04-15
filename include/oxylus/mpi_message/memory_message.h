
#ifndef MEMORY_MESSAGE_H
#define MEMORY_MESSAGE_H
#include <string>
#include <iostream>


#include <oxylus/configuration/Serialization.h>
#include <oxylus/memory/mem_info.h>
#include <oxylus/configuration/configuration_object.h>

namespace rdf{
  namespace bpc{

    class MemoryMessage {
    private:
      /* data */
      int idProcess;
      int memAvailable;
      int memTotal;
      int memFree;
      int batchSize;
      ConfigurationObject* configObject;


      /* --- Serialization --- */
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version) {
            ar & idProcess;
            ar & memTotal;
            ar & memFree;
            ar & memAvailable;
            ar & batchSize;
        }
        /* ==== Serialization ==== */


    public:
      void SetIdProcess(int idProcess){
        this->idProcess = idProcess;
      }
      void SetMemAvailable(int memAvailable){
        this->memAvailable = memAvailable;
      }
      void SetMemTotal(int memTotal){
        this->memTotal = memTotal;
      }
      void SetMemFree(int memFree){
        this->memFree = memFree;
      }
      /* void SetBatchSize(int batchSize){ */
      /*   this->batchSize = batchSize; */
      /* } */
      int GetIdProcess(){
        return this->idProcess;
      }
      int GetMemFree(){
        return this->memFree;
      }
      /* int GetBatchSize(){ */
      /*   return this->batchSize; */
      /* } */
      int GetMemAvailable(){
        return this->memAvailable;
      }
      int GetMemTotal(){
        return this->memTotal;
      }

      int CreateMemoryMessage(){
        MemInfo memInfo(this->configObject);
        this->memTotal = memInfo.GetMemTotal();
        this->memAvailable = memInfo.GetMemAvailable();
        this->memFree = memInfo.GetMemFree();
        this->batchSize = memInfo.GetBatchSize();
        return 0;
      }

      void Print(){
        std::cout << "====Printing memory message ======" << '\n';
        std::cout << "IdProcess: " << idProcess << '\t';
        std::cout << "MemTotal: " << memTotal << '\t';
        std::cout << "MemAvailable: " << memAvailable << '\t';
        std::cout << "MemFree: " << memFree << '\t';
        std::cout << "BatchSize: " << batchSize << '\n';
        std::cout << "-----Printing memory message------" << '\n';
      }

      MemoryMessage (){
        idProcess = 0;
        memFree = 0;
        memTotal = 0;
        memAvailable = 0;
        batchSize = 0;
      }

      MemoryMessage(ConfigurationObject* configObject){
        this->configObject = configObject;
      }

      MemoryMessage(ConfigurationObject* configObject, int idProcess){
        this->configObject = configObject;
        this->idProcess = idProcess;
      }

      MemoryMessage(int idProcess){
        this->idProcess = idProcess;
      }

      virtual ~MemoryMessage(){};

    };

  }
}


#endif

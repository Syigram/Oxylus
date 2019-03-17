#ifndef MEMORY_MESSAGE_H
#define MEMORY_MESSAGE_H
#include <string>
#include <iostream>
#include <oxylus/configuration/Serialization.h>

class MemoryMessage {
private:
  /* data */
  int _idProcess;
  int _memAvailable;
  int _memTotal;
  int _memFree;
  int _batchSize;

  /* --- Serialization --- */
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & _idProcess;
        ar & _memTotal;
        ar & _memFree;
        ar & _memAvailable;
        ar & _batchSize;
    }
    /* ==== Serialization ==== */


public:
  void SetIdProcess(int idProcess){
    this->_idProcess = idProcess;
  };
  void SetMemAvailable(int memAvailable){
    this->_memAvailable = memAvailable;
  };
  void SetMemTotal(int memTotal){
    this->_memTotal = memTotal;
  };
  void SetMemFree(int memFree){
    this->_memFree = memFree;
  };
  void SetBatchSize(int batchSize){
    this->_batchSize = batchSize;
  };
  int GetIdProcess(){
    return this->_idProcess;
  };
  int GetMemFree(){
    return this->_memFree;
  };
  int GetBatchSize(){
    return this->_batchSize;
  };
  int GetMemAvailable(){
    return this->_memAvailable;
  };
  int GetMemTotal(){
    return this->_memTotal;
  };

  void Print(){
    std::cout << "IdProcess: " << _idProcess << '\t';
    std::cout << "MemTotal: " << _memTotal << '\t';
    std::cout << "MemAvailable: " << _memAvailable << '\t';
    std::cout << "MemFree: " << _memFree << '\t';
    std::cout << "BatchSize: " << _batchSize << '\n';
  }

  MemoryMessage (){
    _idProcess = 0;
    _memFree = 0;
    _memTotal = 0;
    _memAvailable = 0;
    _batchSize = 0;
  };

  virtual ~MemoryMessage (){};
};

#endif

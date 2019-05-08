#ifndef IMAGE_ALLOCATION_MESSAGE_H
#define IMAGE_ALLOCATION_MESSAGE_H

#include <string>
#include <iostream>

#include <oxylus/configuration/Serialization.h>


class ImageBatchMessage {
private:

    /* data */
    int _idProcess;
    int _indexStart;
    int _indexEnd;
    int _batchSize;

  /* --- Serialization --- */
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {

        ar & _idProcess;
        ar & _indexStart;
        ar & _indexEnd;
        ar & _batchSize;
    }
    /* ==== Serialization ==== */

public:
  void SetIdProcess(int idProcess){
    this->_idProcess = idProcess;
  }
  int GetIdProcess () const {
    return this->_idProcess;
  }
  void SetIndexStart(int start){
    this->_indexStart = start;
  }
  void SetIndexEnd(int end){
    this->_indexEnd = end;
  }
  void SetBatchSize(int batchSize){
    this->_batchSize = batchSize;
  }

  int GetIndexStart() const {
    return this->_indexStart;
  }
  int GetIndexEnd() const {
    return this->_indexEnd;
  }
  int GetBatchSize() const {
    return this->_batchSize;
  }

  void Print(){
    std::cout << "IdProcess: " << _idProcess << '\t';
    std::cout << "BatchSize: " << _batchSize << '\t';
    std::cout << "IndexStart: " << _indexStart << '\t';
    std::cout << "IndexEnd: " << _indexEnd << '\n';
  }

  ImageBatchMessage(int p, int s, int e, int b) :
    _idProcess(p), _indexStart(s), _indexEnd(e), _batchSize(b)
  {}

  ImageBatchMessage (){
    _idProcess = 0;
    _indexStart = 0;
    _indexEnd = 0;
    _batchSize = 0;
  }

  ImageBatchMessage (const ImageBatchMessage& copyObj){ /* copy-constructor */
    _idProcess = copyObj.GetIdProcess();
    _indexStart = copyObj.GetIndexStart();
    _indexEnd = copyObj.GetIndexEnd();
    _batchSize = copyObj.GetBatchSize();
  }
  virtual ~ImageBatchMessage (){}
};


#endif


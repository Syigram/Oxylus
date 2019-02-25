#ifndef IMAGE_ALLOCATION_MESSAGE_H
#define IMAGE_ALLOCATION_MESSAGE_H


class ImageAllocationMessage {
private:
  /* data */
  int _indexStart;
  int _indexEnd;
  int _batchSize;

public:

  void SetIndexStart(int start){
    this->_indexStart = start;
  };
  void SetIndexEnd(int end){
    this->_indexEnd = end;
  };
  void SetBatchSize(int batchSize){
    this->_batchSize = batchSize;
  };

  int GetIndexStart(){
    return this->_indexStart;
  };
  int GetIndexEnd(){
    return this->_indexEnd;
  };
  int GetBatchSize(){
    return this->_batchSize;
  };

  ImageAllocationMessage (){};
  virtual ~ImageAllocationMessage (){};
};

#endif

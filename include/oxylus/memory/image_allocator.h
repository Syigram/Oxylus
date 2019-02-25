#ifndef IMAGE_ALLOCATOR_H
#define IMAGE_ALLOCATOR_H

// #define IMAGE_AVG_SIZE 20 /* size in kB */

class ImageAllocator {
private:
  /* data */
  int _indexStart;
  int _indexEnd;
  int CalcBatchSize();
public:
  void SetIndexStart(int start){
    this->_indexStart = start;
  };
  void SetIndexEnd(int end){
    this->_indexEnd = end;
  };
  int GetMaxBatchSize(); /* returns total images that a PC can process */
  ImageAllocator (){};
  virtual ~ImageAllocator (){};
};

#endif

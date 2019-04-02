#ifndef FILE_READER_H
#define FILE_READER_H

#include <string>



class FileReader {
  private:
    /* data */

    int indexStart;
    int indexEnd;
    std::string basePath;

  public:
    FileReader();
    FileReader(int indexStart, int indexEnd);
    int ReadFiles();
    void SetIndexStart(int index){
      this->indexStart = index;
    }

    void SetIndexEnd(int index){
      this->indexEnd = index;
    }


    int GetIndexStart(){
      return this->indexStart;
    }

    int GetIndexEnd(){
      return this->indexEnd;
    }

    virtual ~FileReader(){}

};

#endif

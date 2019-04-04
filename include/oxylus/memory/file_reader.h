#ifndef FILE_READER_H
#define FILE_READER_H

#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

class FileReader {
  private:
    /* data */

    int indexStart;
    int indexEnd;
    std::string baseFolder;
    std::string depthFolder;
    std::string labelFolder;
    std::string fileExtension;
    std::string fileBasePath;

  public:
    FileReader();
    FileReader(int indexStart, int indexEnd);
    int ReadImage(std::string fullPath);
    int ReadImages();
    int ReadImages(int indexStart, int indexEnd);
    cv::Mat& ScanImage(cv::Mat& I);
    cv::Mat& ShowImage(cv::Mat& image);
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

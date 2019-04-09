#ifndef FILE_READER_H
#define FILE_READER_H

#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>


#include <oxylus/image/image_operations.h>


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
    int ProcessImages(int indexStart, int indexEnd);
    static int ReadImageTest(std::string fullPath);
    cv::Mat_<ushort> ReadDepthImage(std::string filename);
    cv::Mat_<uchar> ReadLabelImage(std::string filename);
    int ReadImage(std::string fullPath, MapPalette& palette);
    int ReadImages();
    int ReadImages(int indexStart, int indexEnd);
    cv::Mat ScanImage(cv::Mat& image, MapPalette& palette);
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

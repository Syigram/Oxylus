#ifndef FILE_READER_H
#define FILE_READER_H

#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>

#include <oxylus/configuration/configuration_object.h>
#include <oxylus/image/image_operations.h>

namespace rdf{

  namespace bpc{

    class FileReader {
      public:
        FileReader();
        FileReader(ConfigurationObject* configObject);
        FileReader(int indexStart, int indexEnd);
        int ProcessImages(int indexStart, int indexEnd);
        static int ReadImageTest(std::string fullPath);
        cv::Mat_<ushort> ReadDepthImage(std::string filename);
        cv::Mat_<uchar> ReadLabelImage(std::string filename, int rows,
                                       int cols, MapPalette& palette);
        int ReadImage(std::string fullPath, MapPalette& palette);
        int ReadImages();
        std::shared_ptr<ImagesVector> ReadImages(int indexStart, int indexEnd);
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

      private:
        /* data */

        ConfigurationObject * configObject;
        int indexStart;
        int indexEnd;
        std::string baseFolder;
        std::string depthFolder;
        std::string labelFolder;
        std::string fileExtension;
        std::string fileBasePath;


    };

  }

}


#endif

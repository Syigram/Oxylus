#include <set>

#include <oxylus/memory/file_reader.h>
#include <oxylus/memory/helper.h>

using namespace cv;
using namespace std;

FileReader::FileReader(){
   this->baseFolder = "/home/agonzalez/tfg/oxylus_images/";
   this->labelFolder = "label/";
   this->depthFolder = "depth/";
   this->fileExtension = ".png";
   this->fileBasePath = "Data_";
}



FileReader::FileReader(int start, int end){
  this->indexStart = start;
  this->indexEnd = end;
}

int FileReader::ReadImages(int start, int end){
  while (start <= end){
    std::string fileName = Helper::ImageFileNameHandler(this->fileBasePath, start);
    std::string fullPath = baseFolder + labelFolder + fileName + fileExtension;
    /* std::string fullPath = baseFolder + depthFolder + fileName + fileExtension; */
    std::cout << fullPath << std::endl;
    this->ReadImage(fullPath);
    start++;
  }
  return 0;
}

int FileReader::ReadImage(std::string fullPath){
  Mat image = imread(fullPath, CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
  int depth = image.depth();
  cout << "Depth: " << depth << endl;
  if(!image.data){
      cout <<  "Could not open or find the image" << std::endl ;
      return -1;
  }
  namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
  imshow( "Display window", image );                   // Show our image inside it.

  waitKey(0);                                          // Wait for a keystroke in the window
  ScanImage(image);
  return 0;
}


Mat& FileReader::ScanImage(Mat& image){
    // accept only char type matrices
    /* CV_Assert(image.depth() == CV_8U); */
    std::set<int> pixelValues;
    int rows, cols;
    const int channels = image.channels();
    if (!image.isContinuous())
      image = image.clone();
    rows = image.rows;
    cols = image.cols;
    cout << "type: " << image.type() << endl;
    unsigned char* pixelPtr = (unsigned char*)image.data;
    Scalar_<unsigned char> bgrPixel;
    unsigned char pixel;
    /* cout << "M = "<< endl << " "  << image << endl << endl; */
    for (int i = 0; i < rows; i++){
      const uchar* current = image.ptr<uchar>(i);
      for (int j = 0; j < channels * cols; j++){

        /* int val = (int) current[i]; */
        /* pixelValues.insert(val); */

        Vec3b intensity = image.at<Vec3b>(i, j);
          for(int k = 0; k < image.channels(); k++) {
            int col = static_cast<unsigned char>(intensity.val[k]);
            pixelValues.insert(col);
          }

        /* bgrPixel.val[0] = pixelPtr[i*cols*channels + j*channels + 0]; */
        /* bgrPixel.val[1] = pixelPtr[i*cols*channels + j*channels + 0]; */
        /* bgrPixel.val[2] = pixelPtr[i*cols*channels + j*channels + 0]; */

        /* if (bgrPixel.val[0] > 0 || bgrPixel.val[1] > 0 || bgrPixel.val[2] > 0){ */
        /*   pixelValues.insert(bgrPixel.val[1]); */
        /*   pixelValues.insert(bgrPixel.val[2]); */
        /*   pixelValues.insert(bgrPixel.val[3]); */

          /* cout << "pixel1: " << (int) bgrPixel.val[0] << endl; */
          /* cout << "pixel2: " << (int) bgrPixel.val[1] << endl; */
          /* cout << "pixel3: " << (int) bgrPixel.val[2] << endl; */
        /* } */
       /* pixelValues.insert(val); */
       /* if (val > 0) */
       /* cout << "am val: " << val << endl; */
      }
    }

    for (auto pixel: pixelValues){
      cout << "pixel: " << (int) pixel << endl;
    }

    return image;
}

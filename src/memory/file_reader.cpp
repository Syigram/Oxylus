#include <set>
#include <oxylus/memory/file_reader.h>
#include <oxylus/memory/helper.h>
#include <oxylus/misc/custom_exception.h>


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

int FileReader::ProcessImages(int indexStart, int indexEnd) {
  MapPalette palette = ImageOperations::InitializePalette();
  while (indexStart <= indexEnd){
    std::string fileName = Helper::ImageFileNameHandler(this->fileBasePath, indexStart);
    std::string fullPath = baseFolder + labelFolder + fileName + fileExtension;
    /* std::string fullPath = baseFolder + depthFolder + fileName + fileExtension; */
    /* std::cout << fullPath << std::endl; */
    this->ReadImage(fullPath, palette);
    indexStart++;
  }
  return 0;
}

int FileReader::ReadImages(int start, int end){
  MapPalette palette = ImageOperations::InitializePalette();
  while (start <= end){
    std::string fileName = Helper::ImageFileNameHandler(this->fileBasePath, start);
    std::string fullPath = baseFolder + labelFolder + fileName + fileExtension;
    /* std::string fullPath = baseFolder + depthFolder + fileName + fileExtension; */
    std::cout << fullPath << std::endl;
    this->ReadImage(fullPath, palette);
    start++;
  }
  ImageOperations::PrintPalette(palette);
  return 0;
}

int FileReader::ReadImage(std::string fullPath, MapPalette& palette){
  cv::Mat image = cv::imread(fullPath, CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
  int depth = image.depth();
  std::cout << "Depth: " << depth << std::endl;
  if(!image.data){
    std::cout <<  "Could not open or find the image" << std::endl ;
      return -1;
  }
  cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );// Create a window for display.
  cv::imshow( "Display window", image );                   // Show our image inside it.
  ScanImage(image, palette);
  return 0;
}

int FileReader::ReadImageTest(std::string fullPath) {

  cv::Mat image = cv::imread(fullPath, CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
  int depth = image.depth();
  int type = image.type();
  std::cout << "Depth: " << depth << std::endl;
  std::cout << "Type: " << type << std::endl;
  if(!image.data){
    std::cout <<  "Could not open or find the image" << std::endl ;
      return -1;
  }
  cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );// Create a window for display.
  cv::imshow( "Display window", image );                   // Show our image inside it.
  cv::waitKey(0);
  return 0;
}


cv::Mat FileReader::ScanImage(cv::Mat& image, MapPalette& palette) {

  const int channels = image.channels();
  const int depth = image.depth();

  static const cv::Vec3b Black(0u,0u,0u);


  /* Ensure that at least the black color is in the palete with the
     zero label */
  if (palette.empty()) {
    palette.emplace(std::make_pair(Black,uchar(0)));
  }

  if (depth != CV_8U) {
    throw CustomException("Unexpected depth: it should be uchar");
  }

  /* Two cases: 3 channels -> we have to create the palette
   *            1 channel -> the image is ready, we create a dummy palette
   */


  if ( channels==3) {

    /* Resulting labels */
    cv::Mat_<uchar> labels(image.rows,image.cols);

    /* Find largest label used so far in the palette */
    auto maxLabel =
      std::max_element(palette.begin(),
                       palette.end(),
                       [](const std::pair<cv::Vec3b,uchar>& p1,
                          const std::pair<cv::Vec3b,uchar>& p2) {
                         return p1.second < p2.second;
                       });
    assert(maxLabel != palette.end());
    uchar lastLabel = maxLabel->second;

    for (int i = 0; i < image.rows; i++) {
      for (int j = 0; j < image.cols; j++) {

        const cv::Vec3b& rgb = image.at<cv::Vec3b>(i, j);
        auto it=palette.find(rgb);
        if (it==palette.end()) {
          lastLabel++;
          palette[rgb]=lastLabel; // add a new label to the palette
          labels.at<uchar>(i,j) = lastLabel;
        } else {
          labels.at<uchar>(i,j) = it->second;
        }

      }
    }
    return labels;
  }
  else if (channels==1) {

    for (int i = 0; i < image.rows; i++) {
      for (int j = 0; j < image.cols; j++) {
        const uchar label = image.at<uchar>(i, j);
        palette[cv::Vec3b(label,label,label)] = label;
      }
    }

  } else {
    throw CustomException("Unexpected number of channels: must be 1 or 3");
  }

  return image;
}

cv::Mat_<uchar> FileReader::ReadLabelImage(std::string filename) {
  cv::Mat_<uchar> image = cv::imread(filename);
  const int expectedChannels = 3;
  const int channels = image.channels();
  const int depth = image.depth();
  if(!image.data){
    throw CustomException("Invalid path for file: Could not open or find the image");
  }
  if (channels != expectedChannels){
    throw CustomException("Unexpected channels: Image does not have 3 channels");
  }
  if (depth != CV_8U){
    throw CustomException("Unexpected depth: It should be uchar");
  }
  return image;
}


cv::Mat_<ushort> FileReader::ReadDepthImage(std::string filename) {
  cv::Mat_<uchar> image = cv::imread(filename);
  const int expectedChannels = 1;
  const int channels = image.channels();
  const int depth = image.depth();
  if(!image.data){
    throw CustomException("Invalid path for file: Could not open or find the image");
  }
  if (channels != expectedChannels){
    throw CustomException("Unexpected channels: Image has more than 1 channel");
  }
  if (depth != CV_16U){
    throw CustomException("Unexpected depth: It should be ushort");
  }
  return image;

}


/* Mat& FileReader::ScanImage(Mat& image){ */
/*     // accept only char type matrices */
/*     /1* CV_Assert(image.depth() == CV_8U); *1/ */
/*     std::set<int> pixelValues; */
/*     int rows, cols; */
/*     const int channels = image.channels(); */
/*     if (!image.isContinuous()) */
/*       image = image.clone(); */
/*     rows = image.rows; */
/*     cols = image.cols; */
/*     cout << "type: " << image.type() << endl; */
/*     unsigned char* pixelPtr = (unsigned char*)image.data; */
/*     Scalar_<unsigned char> bgrPixel; */
/*     unsigned char pixel; */
/*     /1* cout << "M = "<< endl << " "  << image << endl << endl; *1/ */
/*     for (int i = 0; i < rows; i++){ */
/*       const uchar* current = image.ptr<uchar>(i); */
/*       for (int j = 0; j < channels * cols; j++){ */

/*         /1* int val = (int) current[i]; *1/ */
/*         /1* pixelValues.insert(val); *1/ */

/*         Vec3b intensity = image.at<Vec3b>(i, j); */
/*         cout << "pixel1: " << (int) intensity.val[0] << '\t'; */
/*         cout << "pixel2: " << (int) intensity.val[1] << '\t'; */
/*         cout << "pixel3: " << (int) intensity.val[2] << endl; */
/*         /1* for(int k = 0; k < image.channels(); k++) { *1/ */
/*         /1*   int col = static_cast<unsigned char>(intensity.val[k]); *1/ */
/*         /1*   pixelValues.insert(col); *1/ */
/*         /1* } *1/ */

/*         /1* bgrPixel.val[0] = pixelPtr[i*cols*channels + j*channels + 0]; *1/ */
/*         /1* bgrPixel.val[1] = pixelPtr[i*cols*channels + j*channels + 1]; *1/ */
/*         /1* bgrPixel.val[2] = pixelPtr[i*cols*channels + j*channels + 2]; *1/ */

/*         /1* if (bgrPixel.val[0] > 0 || bgrPixel.val[1] > 0 || bgrPixel.val[2] > 0){ *1/ */

/*         /1*   cout << "pixel1: " << (int) bgrPixel.val[0] << '\t'; *1/ */
/*         /1*   cout << "pixel2: " << (int) bgrPixel.val[1] << '\t'; *1/ */
/*         /1*   cout << "pixel3: " << (int) bgrPixel.val[2] << endl; *1/ */
/*         /1* } *1/ */
/*        /1* pixelValues.insert(val); *1/ */
/*        /1* if (val > 0) *1/ */
/*        /1* cout << "am val: " << val << endl; *1/ */
/*       } */
/*     } */

/*     return image; */
/* } */

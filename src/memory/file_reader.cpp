#include <set>
#include <oxylus/memory/file_reader.h>
#include <oxylus/memory/helper.h>
#include <oxylus/misc/custom_exception.h>

using namespace rdf::bpc;

FileReader::FileReader(){
}

FileReader::FileReader(ConfigurationObject* configObject){
  this->configObject = configObject;
  this->depthFolder = configObject->GetDepthFolder();
  this->baseFolder = configObject->GetBaseFolder();
  this->labelFolder = configObject->GetLabelFolder();
  this->fileExtension = configObject->GetFileExtension();
  this->fileBasePath = configObject->GetFilePrefix();
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


std::shared_ptr<ImagesVector> FileReader::ReadImages(int start, int end){
  MapPalette palette = ImageOperations::InitializePalette();
  int rows = this->configObject->GetImagesRows();
  int cols = this->configObject->GetImagesCols();
  int batchSize = (start - end) + 1;
  std::shared_ptr<ImagesVector> imagesVector =
                std::make_shared<ImagesVector>();
  while (start <= end){
    std::string fileName = Helper::ImageFileNameHandler(this->fileBasePath, start);
    std::string labelFilename = baseFolder + labelFolder + fileName + fileExtension;
    std::string depthFilename = baseFolder + depthFolder + fileName + fileExtension;
    /* std::cout << labelFilename << std::endl; */
    /* std::cout << depthFilename << std::endl; */
    cv::Mat_<ushort> depthMat = this->ReadDepthImage(depthFilename);
    cv::Mat_<uchar> labelMat = this->ReadLabelImage(labelFilename, rows, cols, palette);
    int imageId = start;
    ImageStructure imageData(this->configObject, depthMat, imageId);
    imageData.SetPointsVector(ImageOperations::GenerateRandomPoints(imageData, depthMat, labelMat, palette));
    imagesVector->push_back(imageData);
    start++;
  }
  /* ImageOperations::PrintPalette(palette); */
  /* ImageOperations::SavePaletteTo("palette.txt", palette); */
  return imagesVector;
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
  for (int i = 0; i < image.rows; i++) {
    for (int j = 0; j < image.cols; j++) {
      int val = (int) image.at<ushort>(i, j);
      ushort test = image.at<ushort>(i, j);
      if (val > 0)
        std::cout << "depth: " << test << std::endl;

    }
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

cv::Mat_<uchar> FileReader::ReadLabelImage(std::string filename, int rows,
                                            int cols, MapPalette& palette) {
  cv::Mat image = cv::imread(filename, CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
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

  /* Resulting labels */
  cv::Mat_<uchar> labels(rows, cols);

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
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
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


cv::Mat_<ushort> FileReader::ReadDepthImage(std::string filename) {
  cv::Mat image = cv::imread(filename, CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
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
  cv::Mat_<ushort> typedMat;
  image.convertTo(typedMat, CV_16U);
  return typedMat;

}


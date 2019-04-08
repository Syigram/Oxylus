#ifndef IMAGE_STRUCTURE_H
#define IMAGE_STRUCTURE_H

#include <map>
#include <set>

class ImageStructure {
  private:
    /* data */
    cv::Mat labelsImage;
    cv::Mat depthImage;

    vector<int> treesId; /* this vector contains the id of the trees that use
                            this image for training */

    std::set<std::pair<int,int>> setOfPoints;

    int rows;
    int cols;
}
#endif /* IMAGE_STRUCTURE_H */

#ifndef FEATURES_H
#define FEATURES_H

#include <opencv2/core/types.hpp>

namespace rdf{
  namespace bpc{

    class Features{
      private:
        cv::Point delta1;
        cv::Point delta2;

      public:

        void SetDelta1(int positionX, int positionY){
          this->delta1.x = positionX;
          this->delta1.y = positionY;
        }

        void SetDelta2(int positionX, int positionY){
          this->delta2.x = positionX;
          this->delta2.y = positionY;
        }

        cv::Point GetDelta1(){
          return this->delta1;
        }

        cv::Point GetDelta2(){
          return this->delta2;
        }

        int GetDelta1X(){
          return this->delta1.x;
        }

        int GetDelta1Y(){
          return this->delta1.y;
        }

        int GetDelta2X(){
          return this->delta2.x;
        }

        int GetDelta2Y(){
          return this->delta2.y;
        }

    };
  }
}

#endif /* FEATURES_H */

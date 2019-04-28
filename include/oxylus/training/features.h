#ifndef FEATURES_H
#define FEATURES_H

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>

#include <oxylus/configuration/Serialization.h>


namespace rdf{
  namespace bpc{

    class Features{
      private:
        cv::Point delta1;
        cv::Point delta2;

        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version) {
          ar & delta1.x;
          ar & delta1.y;
          ar & delta2.x;
          ar & delta2.y;
        }

      public:
        void Print(){
          std::cout << "Delta1X: " << delta1.x << "\tDelta1Y: " << delta1.y << std::endl;
          std::cout << "Delta2X: " << delta2.x << "\tDelta2Y: " << delta2.y << std::endl;
        }

        void SetDelta1(int positionX, int positionY){
          this->delta1.x = positionX;
          this->delta1.y = positionY;
        }

        void SetDelta1X(int positionX){
          this->delta1.x = positionX;
        }

        void SetDelta1Y(int positionY){
          this->delta1.y = positionY;
        }

        void SetDelta2X(int positionX){
          this->delta2.x = positionX;
        }

        void SetDelta2Y(int positionY){
          this->delta2.y = positionY;
        }

        void SetDelta2(int positionX, int positionY){
          this->delta2.x = positionX;
          this->delta2.y = positionY;
        }

        cv::Point GetDelta1() const{
          return this->delta1;
        }

        cv::Point GetDelta2() const{
          return this->delta2;
        }

        int GetDelta1X() const{
          return this->delta1.x;
        }

        int GetDelta1Y() const {
          return this->delta1.y;
        }

        int GetDelta2X() const {
          return this->delta2.x;
        }

        int GetDelta2Y() const {
          return this->delta2.y;
        }

        inline bool operator=(Features& other){
          this->delta1.x = other.GetDelta1X();
          this->delta1.y = other.GetDelta1Y();
          this->delta2.x = other.GetDelta2X();
          this->delta2.y = other.GetDelta2Y();
          return true;
        }

        inline bool operator==(const Features& rhs){
          if (this->delta1.x == rhs.GetDelta1X() &&
              this->delta1.y == rhs.GetDelta1Y() &&
              this->delta2.x == rhs.GetDelta2X() &&
              this->delta2.y == rhs.GetDelta2Y()){
            return true;
          }
          return false;
        }
        inline bool operator!=(const Features& rhs){
          if (this->delta1.x != rhs.GetDelta1X() ||
              this->delta1.y != rhs.GetDelta1Y() ||
              this->delta2.x != rhs.GetDelta2X() ||
              this->delta2.y != rhs.GetDelta2Y()){
            return true;
          }
        return false;
        }
    };
  }
}

#endif /* FEATURES_H */

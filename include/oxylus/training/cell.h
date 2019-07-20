#ifndef CELL_H
#define CELL_H

#include <iostream>

#include <oxylus/configuration/configuration_constants.h>
#include <oxylus/configuration/Serialization.h>

namespace rdf{
  namespace bpc{

    class Cell{
      public:
        Cell();
        Cell(int n);
        Cell(const Cell& orig);
        void AddToHistogram(bool left, int value );
        void AddToLeftHistogram(int index);
        void AddToRightHistogram(int index);
        std::vector<int> GetLeftHistogram() const;
        std::vector<int> GetRightHistogram() const;
        void PrintHistograms();
        void PrintLeft();
        void PrintRight();
        Cell& operator=(const Cell& rhs) {
          this->leftHistogram = rhs.leftHistogram;
          this->rightHistogram = rhs.rightHistogram;
          this->leftHistogramTotal = rhs.leftHistogramTotal;
          this->rightHistogramTotal = rhs.rightHistogramTotal;
          return *this;
        }

        inline bool operator!=(Cell& rhs){
          size_t leftSize = this->leftHistogram.size();
          size_t rightSize = this->rightHistogram.size();
          if (rhs.GetLeftHistogram().size() != leftSize ||
              rhs.GetRightHistogram().size() != rightSize){
            return true;
          }
          for (size_t i = 0; i < rightSize; i++){
            if (this->rightHistogram[i] != rhs.rightHistogram[i]){
              return true;
            }
          }
          for (size_t i = 0; i < leftSize; i++){
            if (this->leftHistogram[i] != rhs.leftHistogram[i]){
              return true;
            }
          }
          return false;
        }
        Cell& Reduce(const Cell& other);
        void SetHistogramsTotalCount();
        virtual ~Cell(){};
        std::vector<int> leftHistogram;      ///< Left histogram
        std::vector<int> rightHistogram;     ///< Right histogram
        int leftHistogramTotal;
        int rightHistogramTotal;
        int totalCount;
        double argMinValue;
      private:
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version) {
          ar & leftHistogram;
          ar & rightHistogram;
          ar & leftHistogramTotal;
          ar & rightHistogramTotal;
          ar & totalCount;
          ar & argMinValue;
        }
    };

  }
}


#endif /* CELL_H */

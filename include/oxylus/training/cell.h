#ifndef CELL_H
#define CELL_H

/* #include <oxylus */

namespace rdf{
  namespace bpc{

    template <class T>
    class Cell{
      public:
        Cell();
        Cell(const Cell& orig);
        void AddToHistogram(bool left, T value );
        void AddToLeftHistogram(T value);
        void AddToRightHistogram(T value);
        virtual ~Cell();
      private:
        std::vector<float> leftHistogram;      ///< Left histogram
        std::vector<float> rightHistogram;     ///< Right histogram
    };

  }
}

#include "cell.tpp"

#endif /* CELL_H */

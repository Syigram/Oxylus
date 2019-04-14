#ifndef CELL_H
#define CELL_H

#include <oxylus/configuration/configuration_constants.h>

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
        virtual ~Cell(){};
      private:
        std::vector<float> leftHistogram;      ///< Left histogram
        std::vector<float> rightHistogram;     ///< Right histogram
    };

    template <class T>
    Cell<T>::Cell(){
      this->leftHistogram.resize(rdf::bpc::constants::BODY_PARTS);
      this->rightHistogram.resize(rdf::bpc::constants::BODY_PARTS);
    }

    template <class T>
    Cell<T>::Cell(const Cell& orig){

    }

    template <class T>
    void Cell<T>::AddToHistogram(bool left, T value){

    }

    template <class T>
    void Cell<T>::AddToLeftHistogram(T value){

    }

    template <class T>
    void Cell<T>::AddToRightHistogram(T value){

    }

  }
}


#endif /* CELL_H */

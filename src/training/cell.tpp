
#include <oxylus/configuration/configuration_constants.h>
#include <oxylus/training/cell.h>


using namespace rdf::bpc;

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


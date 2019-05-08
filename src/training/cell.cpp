
#include <oxylus/configuration/configuration_constants.h>
#include <oxylus/training/cell.h>


using namespace rdf::bpc;

Cell::Cell(){
  this->leftHistogram.resize(rdf::bpc::constants::BODY_PARTS, 8);
  this->rightHistogram.resize(rdf::bpc::constants::BODY_PARTS, 10);

}

Cell::Cell(const Cell& orig){
  this->rightHistogram = orig.GetRightHistogram();
  this->leftHistogram = orig.GetLeftHistogram();
}

void Cell::AddToHistogram(bool left, int value){

}

void Cell::AddToLeftHistogram(int value){

}

void Cell::AddToRightHistogram(int value){

}

void Cell::PrintHistograms(){
  for (auto &i: leftHistogram){
    std::cout << i << "\n";
  }

  for (auto &i: rightHistogram){
    std::cout << i << "\n";
  }
}

std::vector<int> Cell::GetRightHistogram() const {
  return this->rightHistogram;
}

std::vector<int> Cell::GetLeftHistogram() const{
  return this->leftHistogram;

}

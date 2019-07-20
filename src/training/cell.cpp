
#include <oxylus/configuration/configuration_constants.h>
#include <oxylus/training/cell.h>


using namespace rdf::bpc;

Cell::Cell(){
  this->leftHistogramTotal = 0;
  this->rightHistogramTotal = 0;
  this->totalCount = 0;
  this->argMinValue = 0.0;
  this->leftHistogram.resize(rdf::bpc::constants::BODY_PARTS, 0);
  this->rightHistogram.resize(rdf::bpc::constants::BODY_PARTS, 0);
}

Cell::Cell(int n) {
  this->leftHistogram.resize(n, 0);
  this->rightHistogram.resize(n, 0);
  this->leftHistogramTotal = 0;
  this->rightHistogramTotal = 0;
  this->totalCount = 0;
  this->argMinValue = 0.0;
}

Cell::Cell(const Cell& orig){
  this->rightHistogram = orig.GetRightHistogram();
  this->leftHistogram = orig.GetLeftHistogram();
  this->leftHistogramTotal = orig.leftHistogramTotal;
  this->rightHistogramTotal = orig.rightHistogramTotal;
  this->totalCount = orig.totalCount;  
}

void Cell::AddToHistogram(bool left, int value){

}

void Cell::AddToLeftHistogram(int index){
  this->leftHistogram[index] += 1;
}

void Cell::AddToRightHistogram(int index){
  this->rightHistogram[index] += 1;
}

void Cell::PrintRight(){
  for (auto &i: rightHistogram){
    std::cout << i << "\n";
  }
}

void Cell::PrintLeft(){
  for (auto &i: leftHistogram){
    std::cout << i << "\n";
  }
}

void Cell::PrintHistograms(){
  PrintRight();
  PrintLeft();
}

std::vector<int> Cell::GetRightHistogram() const {
  return this->rightHistogram;
}

std::vector<int> Cell::GetLeftHistogram() const{
  return this->leftHistogram;

}

Cell& Cell::Reduce(const Cell& other) {
  for (int k = 0; k < constants::BODY_PARTS; k++) {
    this->leftHistogram[k] += other.leftHistogram[k];
    this->rightHistogram[k] += other.rightHistogram[k];
  }
  return *this;
}

void Cell::SetHistogramsTotalCount() {
  for (int k = 0; k < constants::BODY_PARTS; k++) {
    leftHistogramTotal += this->leftHistogram[k];
    rightHistogramTotal += this->rightHistogram[k];
  }
  this->totalCount = leftHistogramTotal + rightHistogramTotal;
}


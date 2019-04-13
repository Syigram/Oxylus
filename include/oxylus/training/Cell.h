#ifndef CELL_H
#define CELL_H

namespace rdf{
  namespace bpc{

    class Cell{
      public:
        Cell();
        Cell(const Cell& orig);
        std::vector<float> left;      ///< Left histogram
        std::vector<float> right;     ///< Right histogram
        virtual ~Cell();


    };
  }
}

#endif /* CELL_H */

#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <string>

#include <oxylus/training/tree.h>

namespace rdf {
  namespace bpc {
    class Predictor {
      public:
        Predictor();
        void SerializeTrees();
                
        virtual ~Predictor();
      private:
        void SerializeTree(std::string filename);
        Tree tree1;
        Tree tree2;
        Tree tree3;

    };
  }
}

#endif /* PREDICTOR_H */

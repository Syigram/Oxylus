#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <string>

#include <oxylus/configuration/configuration_object.h>
#include <oxylus/training/tree.h>

namespace rdf {
  namespace bpc {
    class Predictor {
      public:
        Predictor();
        void DeserializeTrees(ConfigurationObject* configObject);
        virtual ~Predictor();
      private:
        void DeserializeTree(std::string path);
        Tree tree1;
        Tree tree2;
        Tree tree3;

    };
  }
}

#endif /* PREDICTOR_H */

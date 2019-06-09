#ifndef NODE_H
#define NODE_H

#include <oxylus/configuration/Serialization.h>


namespace rdf {

  namespace bpc {

    class Node {
    public:
      Node();
      Node(const Node& orig);
      virtual ~Node();
      virtual Node * GetLeft() = 0;
      virtual Node * GetRight() = 0;
      virtual void SetLeftNode(Node*) = 0;
      virtual void SetRightNode(Node*) = 0;
      virtual int GetNodeId() = 0;
      Node* rightNode;
      Node* leftNode;

    private:
      friend class boost::serialization::access;
      template <typename Archive>
      void serialize(Archive &ar, const unsigned int version) {
        ar & rightNode; 
        ar & leftNode; 
      }

    };

  }


} /* rdf */



#endif /* NODE_H */

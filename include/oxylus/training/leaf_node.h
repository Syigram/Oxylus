#ifndef LEAF_NODE_H
#define LEAF_NODE_H

#include <oxylus/training/node.h>


namespace rdf {
  namespace bpc {

    class LeafNode : public Node {
      public:
        LeafNode();
        LeafNode(int nodeId);
        LeafNode(int nodeId, std::vector<int> histogram);

        Node* GetLeft() {
          return this->leftNode;
        }

        Node* GetRight() {
          return this->rightNode;
        }

        void SetLeftNode(Node* node) {
          this->leftNode = node;
        }
        
        void SetRightNode(Node* node) {
          this->rightNode = node;
        }
        
        int GetNodeId();

        std::vector<int> histogram;
        int nodeId;

        virtual ~LeafNode(){};

      private:
        friend class boost::serialization::access;
        template <typename Archive>
        void serialize(Archive &ar, const unsigned int version) {
          ar & boost::serialization::base_object<Node>(*this);
          ar & nodeId; 
          ar & histogram;
        }

    };

  }
}



#endif /* LEAF_NODE_H */

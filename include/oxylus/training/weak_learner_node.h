#ifndef WEAK_LEARNER_NODE_H
#define WEAK_LEARNER_NODE_H

#include <oxylus/training/node.h>
#include <oxylus/training/features.h>

namespace rdf {
  namespace bpc {

    class WeakLearnerNode : public Node {
    public:
      WeakLearnerNode();
      WeakLearnerNode(int id);
      WeakLearnerNode(int nodeId, Features features, int threshold) :
        threshold(threshold), features(features), nodeId(nodeId) {};
      WeakLearnerNode(const WeakLearnerNode& other) {
        this->features = other.features;
        this->threshold = other.threshold;
        this->nodeId = other.nodeId;
      }
      WeakLearnerNode& operator=(const WeakLearnerNode& other) {
        this->features = other.features;
        this->threshold = other.threshold;
        this->nodeId = other.nodeId;
        return *this;
      }

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

      Features features;
      int threshold;
      int nodeId;
      int GetNodeId();

      virtual ~WeakLearnerNode(){};
    private:
      /* data */
      friend class boost::serialization::access;
      template <typename Archive>
      void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<Node>(*this);
        ar & nodeId; 
        ar & features;
        ar & threshold;
      }
    };

  }

}


#endif /* WEAK_LEARNER_NODE_H */

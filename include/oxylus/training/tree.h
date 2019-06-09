#ifndef TREE_H
#define TREE_H

#include <string>
#include <vector>

#include <oxylus/training/node.h>
#include <oxylus/training/weak_learner_node.h>

namespace rdf {
  namespace bpc {

    class Tree {
      public:
        Tree();
        Tree(std::string name);
        Tree(int id);
        Node* root;
        void Insert(Node* node);
        void InsertAfterNode(Node* parentNode, Node* newNode);
        void SaveNode(Node* node);
        void SaveTree(Node* root);
        void PrintNode(Node* node);
        void PrintTree(Node* root);
        int GetParentId(int nodeId);
        int GetNodeLevel(int nodeId);
        Node* GetNextNode(Node* prevNode, int nextNodeId);
        bool IsLeftChild(int prevNodeId, int nextNodeId);
        bool IsRightChild(int prevNodeId, int nextNodeId);
        void SetRoot(Node* node);
        Node* GetRoot();
        virtual ~Tree(){};
      private:
        std::vector<int> GetParentsList(int id);
        int id;
        std::string name;
        friend class boost::serialization::access;
        template <typename Archive>
        void serialize(Archive &ar, const unsigned int version) {
          ar & id;
          ar & root;
          ar & name;
        }


    };

  }
}


#endif /* TREE_H */
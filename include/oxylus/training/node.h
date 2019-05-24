/*
 * File:   Node.h
 * Author: Sygram
 *
 * Class: rdf::Node
 * Needs revision: Maybe. Consult with coordinator.
 *                 More abstract methods could be declared.
 * Description: Pure abstract class for all nodes entities.
 *
 */

#ifndef NODE_H
#define NODE_H


namespace rdf {

  class Node {
  public:
    Node();
    Node(const Node& orig);
    virtual ~Node();
    virtual Node * GetLeft() = 0;
    virtual Node * GetRight() = 0;
  private:

  };

} /* rdf */



#endif /* NODE_H */

#ifndef LOST_SG_NODE
#define LOST_SG_NODE

#include "lost/gl/forward.h"
#include "lost/rg/forward.h"
#include "lost/platform/shared_ptr.h"

namespace lost
{

namespace rg
{

/** can contain and process other nodes.
 */  
struct Node 
{
  string name;
  list<NodePtr> children;
  bool active;

  Node();
  virtual ~Node();
  
  virtual void process(gl::ContextPtr& ctx);
  void add(const NodePtr& child);
  void addFront(NodePtr& child);
  void remove(const NodePtr& child);
  void removeLast();
  void clear();
  static NodePtr create();
  
  NodePtr recursiveFindByName(const string& inName);
  
  void print(); // prints the whole rendergraph from the current node on recursively using DOUT
};

}
}

#endif

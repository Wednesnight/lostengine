#ifndef LOST_SG_NODE
#define LOST_SG_NODE

#include <string>
#include <list>
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
  std::string name;
  bool active;
  Node();
  virtual ~Node();
  
  virtual void process(gl::ContextPtr& ctx);
  void add(const NodePtr& child);
  void addFront(NodePtr& child);
  void remove(const NodePtr& child);
  static NodePtr create();
  std::list<NodePtr> children;
  
  NodePtr recursiveFindByName(const std::string& inName);
  
  void print(); // prints the whole rendergraph from the current node on recursively using DOUT
};

}
}

#endif

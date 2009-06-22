#ifndef LOST_SG_NODE
#define LOST_SG_NODE

#include <string>
#include <list>
#include "lost/platform/shared_ptr.h"

namespace lost
{

namespace gl
{
struct Context;
typedef lost::shared_ptr<Context> ContextPtr;
}
namespace rg
{

/** can contain and process other nodes.
 */
struct Node;
typedef lost::shared_ptr<Node> NodePtr;
  
struct Node 
{
  std::string name;
  bool active;
  Node();
  virtual ~Node();
  
  virtual void process(gl::ContextPtr ctx);
  void add(NodePtr child);
  void addFront(NodePtr child);
  void remove(NodePtr child);
  static NodePtr create();
  std::list<NodePtr> children;
  
  NodePtr recursiveFindByName(const std::string& inName);
};

}
}

#endif

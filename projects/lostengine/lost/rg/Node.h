#ifndef LOST_SG_NODE
#define LOST_SG_NODE

#include <string>
#include <list>

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

  Node();
  virtual ~Node();
  
  virtual void process(gl::ContextPtr ctx);
  void add(NodePtr child);
  static NodePtr create();
  std::list<NodePtr> children;
};

}
}

#endif

#ifndef LOST_SG_NODE
#define LOST_SG_NODE


namespace lost
{
namespace rg
{

struct Node;
typedef lost::shared_ptr<Node> NodePtr;
  
struct Node 
{
  Node();
  virtual ~Node();
};

}
}

#endif

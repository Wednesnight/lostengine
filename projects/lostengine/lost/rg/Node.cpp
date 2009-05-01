#include "lost/rg/Node.h"

namespace lost
{
namespace rg
{

Node::Node()
{
  name = "node"; 
  active = true;
}

Node::~Node()
{
  
}

void Node::add(NodePtr child)
{
  children.push_back(child);
}

void Node::process(gl::ContextPtr ctx)
{
  if(!active) return; 
  
  for(std::list<NodePtr>::iterator i=children.begin(); i!=children.end(); ++i)
  {
    if((*i)->active)
      (*i)->process(ctx);
  }
}

NodePtr Node::create()
{
  return NodePtr(new Node);
}

}
}

#include "lost/sg/Node.h"

namespace lost
{
namespace sg
{

Node::Node()
{
  
}

Node::~Node()
{
  
}

void Node::add(PrimitivePtr primitive)
{
  primitives.push_back(primitive);
}
}
}
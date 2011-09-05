#include "lost/rg/Node.h"
#include "lost/common/Logger.h"
#include "lost/common/NullDeleter.h"

namespace lost
{
  namespace rg
  {

    void printNode(NodePtr n, const string prefix, uint32_t& numNodes, uint32_t& enabledNodes)
    {
      numNodes++;
      if(n->active) enabledNodes++;
      DOUT(prefix << "|-- " << (n->active ? "(+)" : "(-)") << n->name);
      for(list<NodePtr>::iterator i=n->children.begin(); i!=n->children.end(); ++i)
      {
        printNode(*i, prefix+"    ", numNodes, enabledNodes);
      }      
    }

    Node::Node()
    {
      name = "node"; 
      active = true;
    }

    Node::~Node()
    {
      
    }

    void Node::clear()
    {
      children.clear();
    }

    void Node::print()
    {
      uint32_t numNodes, enabledNodes;
      numNodes = enabledNodes = 0;
      printNode(shared_ptr<Node>(this, common::NullDeleter()), string(""), numNodes, enabledNodes);
      DOUT("Num nodes:"<<numNodes<<" enabled:"<<enabledNodes<<" disabled:"<<(numNodes-enabledNodes));
    }

    void Node::add(const NodePtr& child)
    {
      children.push_back(child);
    }

    void Node::addFront(NodePtr& child)
    {
      children.push_front(child);
    }
    
    void Node::remove(const NodePtr& child)
    {
      children.remove(child);
    }

    void Node::removeLast()
    {
      children.pop_back();
    }
      
    void Node::process(gl::ContextPtr& ctx)
    {
      if(!active) return;
//      DOUT(name);
      
      for(list<NodePtr>::iterator i=children.begin(); i!=children.end(); ++i)
      {
        if((*i)->active)
        {
//          DOUT("  " << (*i)->name);
          (*i)->process(ctx);
        }
      }
    }

    NodePtr Node::create()
    {
      return NodePtr(new Node);
    }

    NodePtr Node::recursiveFindByName(const string& inName)
    {
      NodePtr result;
      for(list<NodePtr>::iterator i=children.begin();
          i!=children.end();
          ++i)
      {
        if((*i)->name == inName)
        {
          result = *i;
          break;
        }
        else if((result = (*i)->recursiveFindByName(inName)))
        {
          break;
        }
      }
      return result;
    }

  }
}

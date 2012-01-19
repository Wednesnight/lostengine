/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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

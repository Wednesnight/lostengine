#include "RectPacker.h"
#include <algorithm>
#include "lost/common/Logger.h"

using namespace lost::math;

RectPacker::Node::Node()
{
  rectid = -1;
  rotated = false;
  small = -1;
  large = -1;
}

RectPacker::RectPacker()
{
}

RectPacker::~RectPacker()
{
}

bool compareRects(const RectPacker::SourceRect& r1, const RectPacker::SourceRect& r2)
{
  return (r1.rect.area() > r2.rect.area());
}

void RectPacker::pack(const lost::math::Rect& targetArea,
          const std::vector<lost::math::Rect>& rects,
          bool rotate)
{
  // clear stats
  sumIter = 0;
  sumNodes = 0;
  
  // clear previous result and init to target area
  nodes.clear();
  Node rootNode;
  rootNode.rect = targetArea;
  nodes.push_back(rootNode);
  
  // create the source vector and preserve incming ids in the progress
  // by wrapping the incoming rects in the SourceRect struct
  sourceRects.clear();
  int32_t numRects = rects.size();
  for(int32_t i=0; i<numRects; ++i)
  {
    sourceRects.push_back(SourceRect(rects[i], i));
  }
  
  // sort the source rects by size for better fitting
  sort(sourceRects.begin(), sourceRects.end(), compareRects);
    
  // pack all source rects into the target area
  for(int32_t i=0; i<numRects; ++i)
  {
    if(!pack(i))
    {
      DOUT("couldn't pack "<<i);
    }
  }
  
  // clean up temporary data
  sourceRects.clear();
  recursionStack.clear();
  DOUT("searched "<<(uint32_t)((sumIter/sumNodes)*100)<<"% of all nodes");
  DOUT(" iterations: "<<sumIter<<" nodes:"<<sumNodes);
}

bool RectPacker::exactMatch(const Node& n, const SourceRect& s)
{
  return ((n.rect.width == s.rect.width) && (n.rect.height == s.rect.height));
}

bool RectPacker::fits(const Node& n, const SourceRect& s)
{
  return ((n.rect.width >= s.rect.width) && (n.rect.height >= s.rect.height));
}


bool RectPacker::pack(int32_t sourceRectId)
{
  // try to find an exactly matching node for the given sourceRectId
  // if there is none, we split the current node until we find one
  foundNode = false; // will be set to true if we actually can create a macthing node
  recursionStack.clear();
  recursionStack.push_back(0); // start at the rootNode

  SourceRect& currentSourceRect = sourceRects[sourceRectId];
  // check if current rect is degenerate and abort immediately
  if((currentSourceRect.rect.width == 0) || (currentSourceRect.rect.height == 0))
  {
    DOUT("rejecting degenerate rect");
    return false;
  }

  DOUT("------------------ "<<sourceRectId<<" "<<currentSourceRect.rect);


  // loop while we haven't found a match and still have space
  // the last entry in the stack will be the node we have to work with
  uint32_t iterations = 0;
  while(!foundNode && (recursionStack.size() > 0))
  {
    iterations++;
    int32_t currentNodeIndex = recursionStack.back();
    Node currentNode = nodes[currentNodeIndex]; // currentNode needs to be copied since we change the vector and the memory might be moved
//    DOUT("currentNodeIndex: "<<currentNodeIndex<<" stack size: "<<recursionStack.size()<<" rect: "<<currentNode.rect);
    
    // terminate if the currentNode is not in use yet and matches exactly
    if((currentNode.rectid == -1) && exactMatch(currentNode, currentSourceRect))
    {
        foundNode = true;
//        currentNode.rect = currentSourceRect.rect;
        currentNode.rectid = currentSourceRect.id;
//        DOUT("found match, stack size: "<<recursionStack.size());
        nodes[currentNodeIndex] = currentNode;
    }
    else
    {
      // only split and recurse if node is not in use yet and sourceRect fits into this node
      if((currentNode.rectid == -1) && fits(currentNode, currentSourceRect))
      {
        // make sure currentNode has children
        split(currentNode, currentSourceRect);
        // pop current node and add children if existent
        // add smaller child last so it will get checked first
        recursionStack.pop_back();
        if((currentNode.large != -1) && (nodes[currentNode.large].rectid==-1)) recursionStack.push_back(currentNode.large);
        if((currentNode.small != -1) && (nodes[currentNode.small].rectid==-1)) recursionStack.push_back(currentNode.small);
//        DOUT("stack size after split: "<<recursionStack.size());
//        DOUT("node ids after split: "<<currentNode.small << " " << currentNode.large);
        nodes[currentNodeIndex] = currentNode;
      }
      else
      {
        // just pop the node off the stack and continue
//        DOUT("won't split node that's already in use");
        recursionStack.pop_back();
      }
    }
  }  
  DOUT("iterations: "<<iterations<<" nodes: "<<nodes.size());
  sumIter += iterations;
  sumNodes += nodes.size();
  return foundNode;
}

int32_t RectPacker::newNode(const lost::math::Rect& r)
{
//  DOUT("creating node with rect: "<<r);
  nodes.push_back(Node(r));
  int32_t newNodeIndex = nodes.size();
  newNodeIndex -= 1;
  return newNodeIndex;
}

void RectPacker::split(Node& n, const SourceRect& s)
{
  Rect noderect = n.rect;
  Rect bmprect = s.rect;
  
  float vsplitmax, hsplitmax;
  Rect vl, vr, ht, hb;

  if((n.small != -1) || (n.large != -1))
  {
//    DOUT("already split");
    return;
  }

  if(noderect.width <= bmprect.width) // don't vsplit if width is already minimum fit for bitmap
  {
    vsplitmax = 0;
  }
  else
  {
    // setup rects for vertical split
    vl = Rect(noderect.x, noderect.y, bmprect.width, noderect.height);  // vertical split left rect
    vr = Rect(noderect.x+bmprect.width, noderect.y, noderect.width-bmprect.width, noderect.height); // vertical split right rect
    // find max child area for vsplit
    vsplitmax = std::max(vl.area(), vr.area());
  }

  if(noderect.height <= bmprect.height) // don't split if height is aready minimum fit for bitmap
  {
    hsplitmax = 0;
  }
  else
  {
    // setup rects for horizontal split
    hb = Rect(noderect.x, noderect.y, noderect.width, bmprect.height);
    ht = Rect(noderect.x, noderect.y+bmprect.height, noderect.width, noderect.height-bmprect.height);
    // find max child area for hsplit
    hsplitmax = std::max(ht.area(), hb.area());
  }

  if((vsplitmax == 0) && (vsplitmax == hsplitmax))
  {
//    DOUT("won't split this node!");
    return;
  }

  if(vsplitmax > hsplitmax)
  {
    if(vl.area() < vr.area())
    {
      n.small = newNode(vl);
      n.large = newNode(vr);
    }
    else
    {
      n.small = newNode(vr);
      n.large = newNode(vl);
    }
  }
  else
  {
    if(hb.area() < ht.area())
    {
      n.small = newNode(hb);
      n.large = newNode(ht);
    }
    else
    {
      n.small = newNode(ht);
      n.large = newNode(hb);
    }
  }
  
}
  


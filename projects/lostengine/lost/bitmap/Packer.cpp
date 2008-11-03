#include "lost/bitmap/Packer.h"

using namespace boost;
using namespace lost::math;

namespace lost
{
namespace bitmap
{

Packer::Node::Node(lost::math::Rect inFrame)
: frame(inFrame)
{
}

void Packer::Node::split(boost::shared_ptr<Bitmap> bitmap)
{  
  // there are two ways to split this frame, horizontally or vertically 
  // we need to find the split with the largest child

  // the bitmap rect is always situated at the origin of the current frame
  math::Rect bmprect(frame.x, frame.y, bitmap->width, bitmap->height);
  
  float vsplitmax, hsplitmax;
  Rect vl, vr, ht, hb;
  
  if(frame.width <= bitmap->width) // don't vsplit if width is already minimum fit for bitmap
  {
    vsplitmax = 0;
  }
  else
  {
    // setup rects for vertical split
    vl = Rect(frame.x, frame.y, bmprect.width, frame.height);  // vertical split left rect
    vr = Rect(frame.x+bmprect.width, frame.y, frame.width-bmprect.width, frame.height); // vertical split right rect
    // find max child area for vsplit
    vsplitmax = std::max(vl.area(), vr.area());
  }

  if(frame.height <= bitmap->height) // don't split if height is aready minimum fit for bitmap
  {
    hsplitmax = 0;
  }
  else
  {
    // setup rects for horizontal split
    hb = Rect(frame.x, frame.y, frame.width, bmprect.height);
    ht = Rect(frame.x, frame.y+bmprect.height, frame.width, frame.height-bmprect.height);
    // find max child area for hsplit
    hsplitmax = std::max(ht.area(), hb.area());
  }
   
  if((vsplitmax == 0) && (vsplitmax == hsplitmax))
    DOUT("DANGER, zero rects!");
  
  if(vsplitmax > hsplitmax)
  {
    if(vl.area() < vr.area())
    {
      small.reset(new Node(vl));
      large.reset(new Node(vr));
    }
    else
    {
      small.reset(new Node(vr));
      large.reset(new Node(vl));
    }
  }
  else
  {
    if(hb.area() < ht.area())
    {
      small.reset(new Node(hb));
      large.reset(new Node(ht));
    }
    else
    {
      small.reset(new Node(ht));
      large.reset(new Node(hb));
    }
  }
}

bool Packer::Node::fits(shared_ptr<Bitmap> bmp)  
{
  math::Rect bmprect(0,0,bmp->width, bmp->height);
  return bmprect.fitsInto(frame);
}
  
bool Packer::Node::add(boost::shared_ptr<Bitmap> inBitmap)
{
  bool result = false;
  if(!bitmap && fits(inBitmap) && (inBitmap->area() == frame.area())) // terminate if no bitmap was assigned to this node and the area is exactly the same as the bitmap
  {
    bitmap = inBitmap;
    result = true;
  }
  else if(!bitmap && fits(inBitmap)) // if this node has a bitmap or is not an exact match, add subnodes if necessary and recurse
  {
    bool didSplit = false;
    if(!(small && large) && (frame.area()>1))
    {
      // split current frame to best accomodate bitmap
      split(inBitmap);
//      DOUT("new split: "<<small->frame << " " << large->frame);
      didSplit = true;
    }
    // traverse the children, small first
    if((small && small->add(inBitmap))
       ||
       (large && large->add(inBitmap))
      )
    {
      result = true;
    }
    else
    {
      if(didSplit)
      {
        DOUT("reverting split");
        small.reset();
        large.reset();
      }
    }
  }
  return result;
}
 
void Packer::Node::draw(boost::shared_ptr<Bitmap> inBitmap)
{
  if(bitmap)
    bitmap->draw(frame.x, frame.y, inBitmap);
  if(small)
    small->draw(inBitmap);
  if(large)
    large->draw(inBitmap);
}
  
Packer::Packer(uint32_t width, uint32_t height)
{
  rootNode.reset(new Node(math::Rect(0,0,width,height)));
}

Packer::~Packer()
{
}
  
bool Packer::add(shared_ptr<Bitmap> inBitmap)
{
  if((inBitmap->width==0) || (inBitmap->height == 0))
  {
    DOUT("rejecting degenerate bitmap");
    return false;
  }
  
  DOUT("adding bitmap, frame: " << Rect(0,0,inBitmap->width, inBitmap->height));
  bool result = rootNode->add(inBitmap);
  if(!result)
  {
    DOUT("couldn't pack bitmap");
  }
  return result;
}
  
boost::shared_ptr<Bitmap> Packer::packedBitmap()
{
  boost::shared_ptr<Bitmap> result(new Bitmap(rootNode->frame.width, rootNode->frame.height, Bitmap::COMPONENTS_RGBA));
  result->clear(common::Color(1,0,1,1));
  rootNode->draw(result);
  return result;
}  
  
}
}
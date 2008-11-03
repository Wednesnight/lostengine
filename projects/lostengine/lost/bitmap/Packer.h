#ifndef LOST_BITMAP_PACKER_H
#define LOST_BITMAP_PACKER_H

#include <boost/shared_ptr.hpp>
#include "lost/bitmap/Bitmap.h"
#include "lost/math/Rect.h"

namespace lost
{
namespace bitmap
{
struct Packer
{
  struct Node
  {
    Node(math::Rect inFrame);
    
    lost::math::Rect          frame; // position and size of this node in the global coord system

    boost::shared_ptr<Node>   small;  // left child or null if not split
    boost::shared_ptr<Node>   large; // right child or null if not split
    
    boost::shared_ptr<Bitmap> bitmap; // bitmap if this node is in use or null

    // adds the given bitmap to this node and splits the remaining area
    bool add(boost::shared_ptr<Bitmap> bitmap);
    void split(boost::shared_ptr<Bitmap> bitmap);    
    bool fits(boost::shared_ptr<Bitmap> bmp);
    void draw(boost::shared_ptr<Bitmap> target); // draws itself or its children to the target bitmap
  };
  
  // creates the packer for a bitmap with the given width and height
  Packer(uint32_t width, uint32_t height);
  virtual ~Packer();
  
  bool add(boost::shared_ptr<Bitmap> inBitmap); // adds a new bitmap to the packing structure, splitting nodes if necessary
  bool recursiveAdd(boost::shared_ptr<Packer::Node> node, boost::shared_ptr<Bitmap> bitmap);
  boost::shared_ptr<Bitmap> packedBitmap(); // creates a packed bitmap from all the nodes
  
  boost::shared_ptr<Node> rootNode;
};
}
}

#endif
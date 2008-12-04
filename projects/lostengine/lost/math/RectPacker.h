#ifndef LOST_MATH_RECTPACKER_H
#define LOST_MATH_RECTPACKER_H

#include <vector>
#include "lost/math/Rect.h"
#include <boost/cstdint.hpp>

namespace lost
{
namespace math
{
    
struct RectPacker
{
  struct Node
  {
    Node();
    Node(const lost::math::Rect& r)
    : rect(r)
    {
      rectid = -1;
      rotated = false;
      tiny = -1;
      large = -1;
    }
    
    bool hasBitmap() { return (rectid != -1);}
    bool hasChildren() { return ((tiny != -1) || (large != -1));};
    
    lost::math::Rect rect; // the area this node occupies in the target area
    boost::int32_t rectid; // the index of the rect in the original input vector
    bool rotated;          // true if the original was rotated to fit into the target area
    boost::int32_t tiny;   // index of the tiny child or -1 if no child
    boost::int32_t large;  // index of the large child or -1 if no child
  };
  
  struct SourceRect;
  
  RectPacker();
  ~RectPacker();
  
  /** packs the given rects into the specified target area.
   *  the result is stored in the nodes vector.
   */
  void pack(const lost::math::Rect& targetArea,
            const std::vector<lost::math::Rect>& rects,
            bool rotate,
            bool sort = true);
  
  std::vector<Node> nodes; // holds the packed rects. rectid is the index of the rect in the incoming vector
  
private:
  void checkOverlap();

  bool exactMatch(const Node& n, const SourceRect& s);
  bool RectPacker::fits(const Node& n, const SourceRect& s);
  void split(Node& n, const SourceRect& s); // splits node to accomodate source rect, if necessary and possible
  bool pack(boost::int32_t sourceRectId);
  boost::int32_t newNode(const lost::math::Rect& r);

  std::vector<SourceRect> sourceRects;
  std::vector<boost::int32_t> recursionStack;
  bool foundNode;
  bool hasSpace;
  
  double sumIter;
  double sumNodes;
};
}
}
#endif
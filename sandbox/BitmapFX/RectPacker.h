#ifndef RECTPACKER_H
#define RECTPACKER_H

#include <vector>
#include "lost/math/Rect.h"
#include <stdint.h>

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
      small = -1;
      large = -1;
    }
    
    lost::math::Rect rect;// the area this node occupies in the target area
    int32_t rectid;      // the index of the rect in the original input vector
    bool rotated;         // true if the original was rotated to fit into the target area
    int32_t small;  // index of the small child or -1 if no child
    int32_t large; // index of the large child or -1 if no child
  };

  struct SourceRect
  {
    SourceRect(const lost::math::Rect inRect, int32_t inId)
    :rect(inRect), id(inId)
    {
    }
  
    lost::math::Rect rect; // the original size
    int32_t id; // it's index in the original incoming data
  };
  
  RectPacker();
  ~RectPacker();
  
  /** packs the given rects into the specified target area.
   *  the result is stored in the nodes vector.
   */
  void pack(const lost::math::Rect& targetArea,
            const std::vector<lost::math::Rect>& rects,
            bool rotate);
  
  std::vector<Node> nodes; // holds the packed rects. rectid is the id of the rect in the incoming vector
  
private:
  bool exactMatch(const Node& n, const SourceRect& s);
  bool RectPacker::fits(const Node& n, const SourceRect& s);
  void split(Node& n, const SourceRect& s); // splits node to accomodate source rect, if necessary and possible
  void pack(int32_t sourceRectId);
  int32_t newNode(const lost::math::Rect& r);

  std::vector<SourceRect> sourceRects;
  std::vector<int32_t> recursionStack;
  bool foundNode;
  bool hasSpace;
  
  double sumIter;
  double sumNodes;
};


#endif
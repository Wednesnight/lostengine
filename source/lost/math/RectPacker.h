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

#ifndef LOST_MATH_RECTPACKER_H
#define LOST_MATH_RECTPACKER_H

#include "lost/math/Rect.h"

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
    int32_t rectid; // the index of the rect in the original input vector
    bool rotated;          // true if the original was rotated to fit into the target area
    int32_t tiny;   // index of the tiny child or -1 if no child
    int32_t large;  // index of the large child or -1 if no child
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
            const vector<lost::math::Rect>& rects,
            bool rotate,
            bool sort = true);

  vector<Node> nodes; // holds the packed rects. rectid is the index of the rect in the incoming vector

private:
  void checkOverlap();

  bool exactMatch(const Node& n, const SourceRect& s);
  bool fits(const Node& n, const SourceRect& s);
  void split(Node& n, const SourceRect& s); // splits node to accomodate source rect, if necessary and possible
  bool pack(int32_t sourceRectId);
  int32_t newNode(const lost::math::Rect& r);

  vector<SourceRect> sourceRects;
  vector<int32_t> recursionStack;
  bool foundNode;
  bool hasSpace;

  double sumIter;
  double sumNodes;
};
}
}
#endif

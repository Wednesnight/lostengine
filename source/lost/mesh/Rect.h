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

#ifndef LOST_MESH_RECT_H
#define LOST_MESH_RECT_H

#include "lost/mesh/Mesh.h"
#include "lost/math/Rect.h"

namespace lost
{
namespace mesh
{

  struct Rectangle;
  typedef Rectangle Rect;
  typedef lost::shared_ptr<Rect> RectPtr;

  struct Rectangle : public Mesh
  {
    Rectangle(const math::Rect& rect);

    void createIndices();
    void createVertices(const math::Rect& r);
    void updateSize(const math::Vec2& newSize);

    static RectPtr create(const math::Rect& rect)
    {
      return RectPtr(new Rect(rect));
    }

  };

}
}

#endif

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

#include "lost/math/AABB.h"

namespace lost
{
namespace math
{
AABB::AABB(Vec3 inOrigin, Vec3 inSize)
: pos(inOrigin),
  size(inSize)
{
}

AABB::AABB()
{
  AABB(Vec3(0,0,0), Vec3(0,0,0));
}

// realtime collision detection, p. 79
bool intersect(const AABB& a, const AABB& b)
{
    float t;
    if((t=a.pos.x-b.pos.x) > b.size.x || -t > a.size.x) return false;
    if((t=a.pos.y-b.pos.y) > b.size.y || -t > a.size.y) return false;
    if((t=a.pos.z-b.pos.z) > b.size.z || -t > a.size.z) return false;
    return true;
}

}
}

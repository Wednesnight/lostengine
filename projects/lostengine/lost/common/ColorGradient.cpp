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

#include "lost/common/ColorGradient.h"

namespace lost
{
namespace common
{

bool sortfunc(const ColorPoint& c1, const ColorPoint& c2)
{
  if(c1.coord < c2.coord)
    return true;
  else 
    return false;

}

void ColorGradient::add(const ColorPoint& cp)
{
  colorPoints.push_back(cp);
  std::sort(colorPoints.begin(), colorPoints.end(), sortfunc);
}

Color ColorGradient::colorAt(float coord)
{
  Color result;
  
  ColorPoint before = findNearestColorBefore(coord);
  ColorPoint after = findNearestColorAfter(coord);
  float delta = after.coord - before.coord;
  float current = coord - before.coord;
  
  float br = before.color.r();
  float bg = before.color.g();
  float bb = before.color.b();
  float ba = before.color.a();
  
  float ar = after.color.r();
  float ag = after.color.g();
  float ab = after.color.b();
  float aa = after.color.a();
  
  float dr = ar - br;
  float dg = ag - bg;
  float db = ab - bb;
  float da = aa - ba;
  
  result.r(br + dr*(current/delta));
  result.g(bg + dg*(current/delta));
  result.b(bb + db*(current/delta));
  result.a(ba + da*(current/delta));
  
  return result;
}

ColorPoint ColorGradient::findNearestColorBefore(float coord)
{
  assert(colorPoints.size() > 0);
  ColorPoint result = colorPoints[0];
  for(uint32_t i=0; i<colorPoints.size(); ++i)
  {
    if((colorPoints[i].coord >= result.coord) &&
       (colorPoints[i].coord <= coord))
    {
      result = colorPoints[i];
    }
    else
      break;
  }
  return result;
}

ColorPoint ColorGradient::findNearestColorAfter(float coord)
{
  assert(colorPoints.size() > 0);
  ColorPoint result = colorPoints[0];
  for(uint32_t i=0; i<colorPoints.size(); ++i)
  {
    if(colorPoints[i].coord > coord)
    {
      result = colorPoints[i];
      break;
    }
  }
  return result;
}


}
}

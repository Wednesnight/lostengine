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
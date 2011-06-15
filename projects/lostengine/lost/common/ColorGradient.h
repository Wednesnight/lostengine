#ifndef LOST_COMMON_COLORGRADIENT_H
#define LOST_COMMON_COLORGRADIENT_H

#include "lost/common/Color.h"
#include "lost/common/forward.h"
#include "lost/common/ColorPoint.h"

namespace lost
{
namespace common
{
// describes a one dimensional color gradient.
// coordinates must be in range [0,1] 
// calling colorAt(c) will return the color of the gradient at coordinate c
struct ColorGradient
{
  void add(const ColorPoint& cp);
  Color colorAt(float coord);
  // colors.size() must always be == coords.size()
  vector<ColorPoint> colorPoints;  

  static ColorGradientPtr create() {return ColorGradientPtr(new ColorGradient);}
  
  ColorPoint findNearestColorBefore(float coord);
  ColorPoint findNearestColorAfter(float coord);
};
}
}

#endif
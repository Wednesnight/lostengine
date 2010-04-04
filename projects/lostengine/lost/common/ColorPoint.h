#ifndef LOST_COMMON_COLORPOINT_H
#define LOST_COMMON_COLORPOINT_H

#include "lost/common/Color.h"

namespace lost
{
namespace common
{
struct ColorPoint
{
  ColorPoint() {coord=0.0f;}
  ColorPoint(float c, const Color& col)
  : coord(c), color(col) 
  {
  }
  
  float coord;
  Color color;
};
}
}

#endif
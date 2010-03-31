#ifndef LOST_COMMON_COLORGRADIENT_H
#define LOST_COMMON_COLORGRADIENT_H

#include "lost/common/Color.h"
#include "lost/common/forward.h"
#include <vector>
namespace lost
{
namespace common
{
// describes a one dimensional color gradient.
// coordinates range from [0,1] and will be clamped in add()
// calling colorAt(c) will return the color of the gradient at coordinate c
struct ColorGradient
{
  void add(float coord, const Color& col);
  Color colorAt(float coord);
  // colors.size() must always be == coords.size()
  std::vector<float> coords;
  std::vector<Color> colors;  
};
}
}

#endif
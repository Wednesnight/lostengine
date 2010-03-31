#include "lost/common/ColorGradient.h"

namespace lost
{
namespace common
{

void ColorGradient::add(float coord, const Color& col)
{
  coords.push_back(coord);
  colors.push_back(col);
}

Color ColorGradient::colorAt(float coord)
{
  Color result;
  
  // FIXME: 
  return result;
}


}
}
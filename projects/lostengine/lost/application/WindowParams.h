#ifndef LOST_APPLICATION_WINDOWPARAMS_H
#define LOST_APPLICATION_WINDOWPARAMS_H

#include <string>
#include "lost/math/Rect.h"

namespace lost
{
namespace application
{
    
struct WindowParams
{
public:
  std::string      caption;
  lost::math::Rect rect;

  WindowParams()
  : caption("<unset>"), rect(0,0,640,480)
  {
  }

  WindowParams(const std::string& inCaption,
               const lost::math::Rect& inRect)
  : caption(inCaption),
    rect(inRect)
  {
  }
};

}
}

#endif
#ifndef LOST_GUIRO_VIEW_H
#define LOST_GUIRO_VIEW_H

#include "lost/math/Rect.h"

namespace lost
{
namespace guiro
{

struct View
{
  View() {}
  virtual ~View() {}
  
  lost::math::Rect bounds;
};

}
}

#endif
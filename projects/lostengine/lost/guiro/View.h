#ifndef LOST_GUIRO_VIEW_H
#define LOST_GUIRO_VIEW_H

#include "lost/event/EventDispatcher.h"
#include "lost/math/Rect.h"

namespace lost
{
  namespace guiro
  {

    struct View : public lost::event::EventDispatcher
    {
      View() {}
      virtual ~View() {}
      
      lost::math::Rect bounds;
    };

  }
}

#endif

#ifndef LOST_APPLICATION_TOUCHEVENT_H
#define LOST_APPLICATION_TOUCHEVENT_H

#include "lost/event/Event.h"
#include "lost/math/Vec2.h"

namespace lost
{
  namespace application
  {
    struct TouchEvent;
    typedef lost::shared_ptr<TouchEvent> TouchEventPtr;
    struct Touch;
    typedef lost::shared_ptr<Touch> TouchPtr;

    struct Touch
    {
      lost::math::Vec2  location; // location on screen, origin in the lower left corner, same as OpenGL
      unsigned short    tapCount;  // number of taps for this touch
      double            timeStamp; // should only be used for delta calculation
      
      static TouchPtr create() { return TouchPtr(new Touch()); }
    };

    /** This is a more or less straight forward port of the UIEvent/UITouch combo of UIKit objects.
     *  Certain values are preprocessed/omitted, since we don't reference UIViews in the rest of
     *  the LostEngine.
     */
    struct TouchEvent : public event::Event
    {

      static const event::Type& TOUCHES_BEGAN() { static event::Type d = "touchesBegan";return d;}
      static const event::Type& TOUCHES_MOVED() { static event::Type d = "touchesMoved";return d;}
      static const event::Type& TOUCHES_ENDED() { static event::Type d = "touchesEnded";return d;}
      static const event::Type& TOUCHES_CANCELLED() { static event::Type d = "touchesCancelled";return d;}

      TouchEvent(const event::Type& inType) : Event(inType) { }
      static TouchEventPtr create(const event::Type& inType) { return TouchEventPtr(new TouchEvent(inType)); }
      virtual ~TouchEvent() {}

      unsigned int size()
      {
        return touches.size();
      }

      TouchPtr get(unsigned int idx)
      {
        return touches[idx];
      }

      // contains all touches for the current event. Might contain more than one, depending
      // on how many fingers the user applied to the screen
      vector<TouchPtr> touches;

      // should only be used for delta calculation, since the baseline might differ from the rest of the framework.
      double timestamp;
    };

  }
}

#endif

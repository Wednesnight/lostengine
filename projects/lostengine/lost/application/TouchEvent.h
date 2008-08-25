#ifndef LOST_APPLICATION_TOUCHEVENT_H
#define LOST_APPLICATION_TOUCHEVENT_H

#include "lost/event/Event.h"
#include "lost/math/Vec2.h"
#include <boost/shared_ptr.hpp>
#include <vector>

namespace lost
{
  namespace application
  {

    /** This is a more or less straight forward port of the UIEvent/UITouch combo of UIKit objects.
     *  Certain values are preprocessed/omitted, since we don't reference UIViews in the rest of 
     *  the LostEngine.
     */
    struct TouchEvent : public event::Event
    {  
      struct Touch; 
      
      struct Touch
      {
        lost::math::Vec2  location; // location on screen, origin in the lower left corner, same as OpenGL
        unsigned short    tapCount;  // number of taps for this touch
        double            timeStamp; // should only be used for delta calculation
      };
      
      static const event::Type& TOUCHES_BEGAN() { static event::Type d = "touchesBegan";return d;}  
      static const event::Type& TOUCHES_MOVED() { static event::Type d = "touchesMoved";return d;}  
      static const event::Type& TOUCHES_ENDED() { static event::Type d = "touchesEnded";return d;}  
      static const event::Type& TOUCHES_CANCELLED() { static event::Type d = "touchesCancelled";return d;}  
      
      TouchEvent(const event::Type& inType) : Event(inType) { }
      virtual ~TouchEvent() {}

      unsigned int size()
      {
        return touches.size();
      }

      boost::shared_ptr<lost::application::TouchEvent::Touch> get(unsigned int idx)
      {
        boost::shared_ptr<lost::application::TouchEvent::Touch> result;
        if (idx < touches.size())
          result = touches[idx];
        return result;
      }

      // contains all touches for the current event. Might contain more than one, depending
      // on how many fingers the user applied to the screen
      std::vector<boost::shared_ptr<lost::application::TouchEvent::Touch> > touches;
      
      // should only be used for delta calculation, since the baseline might differ from the rest of the framework.
      double timestamp;  
    };

  }
}

#endif
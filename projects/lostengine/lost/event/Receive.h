#ifndef LOST_EVENT_RECEIVE_H
#define LOST_EVENT_RECEIVE_H

#include <boost/function.hpp>
#include "lost/event/Event.h"
#include <stdexcept>

namespace lost
{
  namespace event
  {

    /** since the event system only deals with Event*, all client code
     * would have to cast the incoming types. This is not acceptable most of the times since
     * the interfaces would lose information. In order to ease the handling, you can decorate
     * your handler function with receive<>() upon registration. Here's an example:
     * 
     * void myHandler(shared_ptr<MouseEvent> mev) {}
     * dispatcher.addEventListener(MouseEvent::MOUSE_DOWN, myHandler); // this would fail since cast from Event* to MouseEvent* can't be performed automatically
     * dispatcher.addEventListener(MouseEvent::MOUSE_DOWN, receive<MouseEvent>(myHandler)); // this will work since receive<>() can actually receive an Event*, but will perform the cast for you and check the result.
     */
    template<typename TO>
    struct receive
    {
      boost::function<void(lost::shared_ptr<TO>)> func;
      receive(boost::function<void(lost::shared_ptr<TO>)> f) : func(f) {}
      void operator()(lost::shared_ptr<Event> ev)
      {
        lost::shared_ptr<TO> p = lost::dynamic_pointer_cast<TO>(ev);
        if(!p) {throw std::runtime_error("couldn't cast event pointers");}
        func(p);
      }  
    };

  }
}

#endif

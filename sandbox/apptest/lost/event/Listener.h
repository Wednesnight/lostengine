#ifndef LOST_EVENT_LISTENER_H
#define LOST_EVENT_LISTENER_H

#include "lost/event/Event.h"

namespace lost
{
namespace event
{

struct Listener
{
  virtual void call(EventPtr event) = 0;
};

template<typename Class, typename EvType>
struct TypedListener : public Listener
{
  Class* _target;
  void(Class::*_method)(const lost::shared_ptr<EvType>&);
  
  TypedListener(Class* target, void(Class::*method)(const lost::shared_ptr<EvType>&))
  {
    _target = target;
    _method = method;
  }
  
  virtual ~TypedListener()
  {
  }
  
  void call(EventPtr event) 
  {
    (*_target.*_method)(static_pointer_cast<EvType>(event));
  };
};

template<typename TargetClass, typename EventType>
lost::event::ListenerPtr makeListener(TargetClass* target, void(TargetClass::*method)(const lost::shared_ptr<EventType>&))
{
  return lost::event::ListenerPtr(new lost::event::TypedListener<TargetClass, EventType>(target, method));
}

}
}

#endif

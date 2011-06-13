#ifndef LOST_EVENT_LISTENER_H
#define LOST_EVENT_LISTENER_H

#include "lost/event/Event.h"

namespace lost
{
namespace event
{

struct Listener
{
  virtual void call(Event* payload) = 0;
};

template<typename Class, typename EvType>
struct TypedListener : public Listener
{
  Class* _target;
  void(Class::*_method)(const EvType*);
  
  TypedListener(Class* target, void(Class::*method)(const EvType*))
  {
    _target = target;
    _method = method;
  }
  
  virtual ~TypedListener()
  {
  }
  
  void call(Event* payload) 
  {
    (*_target.*_method)(static_cast<EvType*>(payload));
  };
};

}
}

#endif

/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef LOST_EVENT_LISTENER_H
#define LOST_EVENT_LISTENER_H

#include "lost/event/Event.h"

namespace lost
{
namespace event
{

struct Listener
{
  virtual void call(const EventPtr& event) = 0;
};

template<typename Class, typename EvType>
struct TypedListener : public Listener
{
  Class* _target;
  void(Class::*_method)(lost::shared_ptr<EvType>&);
  
  TypedListener(Class* target, void(Class::*method)(lost::shared_ptr<EvType>&))
  {
    _target = target;
    _method = method;
  }
  
  virtual ~TypedListener()
  {
  }
  
  void call(const EventPtr& event) 
  {
    (*_target.*_method)(static_pointer_cast<EvType>(event));
  };
};

template<typename Class, typename EvType>
struct TypedConstListener : public Listener
{
  Class* _target;
  void(Class::*_method)(const lost::shared_ptr<EvType>&);
  
  TypedConstListener(Class* target, void(Class::*method)(const lost::shared_ptr<EvType>&))
  {
    _target = target;
    _method = method;
  }
  
  virtual ~TypedConstListener()
  {
  }
  
  void call(const EventPtr& event) 
  {
    (*_target.*_method)(static_pointer_cast<EvType>(event));
  };
};


template<typename TargetClass, typename EventType>
lost::event::ListenerPtr makeListener(TargetClass* target, void(TargetClass::*method)(lost::shared_ptr<EventType>&))
{
  return lost::event::ListenerPtr(new lost::event::TypedListener<TargetClass, EventType>(target, method));
}

template<typename TargetClass, typename EventType>
lost::event::ListenerPtr makeListener(TargetClass* target, void(TargetClass::*method)(const lost::shared_ptr<EventType>&))
{
  return lost::event::ListenerPtr(new lost::event::TypedConstListener<TargetClass, EventType>(target, method));
}

}
}

#endif

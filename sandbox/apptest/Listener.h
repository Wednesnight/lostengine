#ifndef L_LISTENER_H
#define L_LISTENER_H

struct Event;

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

#endif
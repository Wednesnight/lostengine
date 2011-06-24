#ifndef LOST_EVENT_POOL_H
#define LOST_EVENT_POOL_H

#include "lost/event/forward.h"
#include "lost/event/EventType.h"
#include "tinythread.h"
#include "lost/event/Event.h"

namespace lost
{
namespace event
{

template<typename YOURCLASS> const char* classTypeName() { static const char* d = typeid(YOURCLASS).name(); return d; }

struct Pool
{
  static Pool* instance();
  
  // attempts to find an unused Event object of given type, creates one if necessary.
  // constructor is not necessarily called, you have to initialise it yourself.
  // Only event objects with refcount 0 will be reused. 
  // always returns event objects with refcount 1.
  // for this to work, there has to be a lost::event::classTypeName<EvType>() specialisation.
  // Easiest implementation:
  // template<YOURCLASS> const char* classTypeName() { static const char* d = typeid(YOURCLASS).name(); return d; }
  // @param t this is here for legacy reasons, not strictly required for pool created objects
  template<typename EvType>
  EvType* borrowEvent(event::Type t)
  {
    tthread::lock_guard<tthread::mutex> lock(_mutex);

    EvType* result = NULL;
    const char* ctn = classTypeName<EvType>();
    Event* temp = eventForType(ctn);
    if(!ctn)
    {
      result = new EvType(t);
      type2pool[ctn].push_back(result);

    }
    result->refcount = 1;
    return result;
  }
  
  // call this if you don't need a previously borrowed Event object anymore.
  // decrements refcount by 1. 
  void returnEvent(Event* inEvent)
  {
    tthread::lock_guard<tthread::mutex> lock(_mutex);

    inEvent->refcount--;
  }



  Pool();
  virtual ~Pool();  
  
  
  
private:  
  Event* eventForType(const char* inType);
  tthread::mutex _mutex;
  
  typedef vector<Event*> EventVector;
  typedef map<const char*, EventVector> TypeMap;
  TypeMap type2pool;
};
}
}

#endif
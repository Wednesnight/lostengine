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
    result = eventForType(ctn);
    if(!result)
    {
      result = new EvType(t);
      type2pool[ctn].push_back(result);

    }
    result->refcount++;
    logStats();
    return result;
  }
  
  // call this if you don't need a previously borrowed Event object anymore.
  // decrements refcount by 1. 
  void returnEvent(Event* inEvent)
  {
    tthread::lock_guard<tthread::mutex> lock(_mutex);
    inEvent->refcount--;
    logStats();
  }



  Pool();
  virtual ~Pool();  
  
  
  
private:  
  Event* eventForType(const char* inType);
  void logStats();
  tthread::mutex _mutex;
  
  typedef vector<Event*> EventVector;
  typedef map<const char*, EventVector> TypeMap;
  TypeMap type2pool;
};
}
}

#endif

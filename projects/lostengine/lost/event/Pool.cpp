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

#include "lost/event/Pool.h"
#include <boost/thread/once.hpp>
#include "lost/common/Logger.h"
#include "lost/event/Event.h"

namespace lost
{
namespace event
{

Pool::Pool()
{
}

Pool::~Pool()
{
}

static Pool* _globalEventPool = NULL;

void initGlobalEventPool()
{
  DOUT("+++ initGlobalEventPool");
  _globalEventPool = new Pool();
}

static boost::once_flag lostEventPoolOnceFlag = BOOST_ONCE_INIT;

Pool* Pool::instance()
{
  boost::call_once(initGlobalEventPool, lostEventPoolOnceFlag);
  return _globalEventPool;  
}

/*Event* Pool::createEvent(event::Type inType)
{
  tthread::lock_guard<tthread::mutex> lock(_mutex);
  
  Event* result = eventForType(typeid(event::Event).name());
  if(!result)
  {
    result = new Event(inType);
  }
  
  return result;
}*/

/*void Pool::releaseEvent(Event* inEvent)
{
  tthread::lock_guard<tthread::mutex> lock(_mutex);
  inEvent->refcount--;
}*/

Event* Pool::eventForType(const char* inType)
{
  Event* result = NULL;
  
  vector<Event*> pool = type2pool[inType];
  vector<Event*>::iterator pos;
  for(pos = pool.begin(); pos != pool.end(); ++pos)
  {
    if((*pos)->refcount == 0)
    {
      result = *pos;
      break;
    }
  }
  
  return result;
}

void Pool::logStats()
{
  for(TypeMap::iterator pos=type2pool.begin();
      pos!= type2pool.end();
      ++pos)
  {
    DOUT(pos->first << " - " << pos->second.size());
  }
}


}
}

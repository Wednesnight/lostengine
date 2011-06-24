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


}
}

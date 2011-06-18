#ifndef LOST_EVENT_POOL_H
#define LOST_EVENT_POOL_H

#include <boost/thread/once.hpp>
#include "lost/event/Event.h"
#include "lost/event/Handle.h"

namespace lost
{
namespace event
{

// there is only one event pool for the whole application
struct Pool
{
  Pool();
  ~Pool();
  
  static Pool* instance();
  
  
  struct SubPool
  {
    map<Event*,uint32_t> refcount;
    
    uint32_t numEvents() {return refcount.size(); }
  };
  
  typedef map<const char*, SubPool*> SubPoolMap;
  map<const char*, SubPool*> typeName2subPool;
  
  template<typename EvType>
  SubPool* findSubPool() // creates a subpool if necessary, always returns valid result
  {
    SubPool* result = NULL;
    const char* subpoolName = typeid(TypedHandle<EvType>).name();
    SubPoolMap::iterator pos = typeName2subPool.find(subpoolName);
    if(pos == typeName2subPool.end())
    {
      result = new SubPool();
      typeName2subPool[subpoolName] = result;
    }
    else {
      result = pos->second;
    }
    return result;
  }
  
  template<typename EvType>
  TypedHandle<EvType> createEvent(const event::Type& inType)
  {
    TypedHandle<EvType> result;
    SubPool* sp = findSubPool<EvType>();
    std::cout << "subpool " << typeid(TypedHandle<EvType>).name() << " has " << sp->numEvents() << " events" << std::endl;
    result.event = new EvType(inType);
    result.pool = this;
    std::cout << "creating handle for type " << typeid(TypedHandle<EvType>).name() << std::endl;
    return result;
  }
    
};


}
}

#endif
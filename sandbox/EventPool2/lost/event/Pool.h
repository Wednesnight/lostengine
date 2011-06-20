#ifndef LOST_EVENT_POOL_H
#define LOST_EVENT_POOL_H

#include "lost/event/Event.h"
#include "tinythread.h"

namespace lost
{
namespace event
{

template<typename EvType>
struct TypedHandle 
{
  EvType* event;

  EvType* operator->()
  {
    return static_cast<EvType*>(event);
  }

  TypedHandle()
  {
    event = NULL;
  }
  
  TypedHandle(const TypedHandle<EvType>& other)
  {
    if(other.event)
    {
      other.event->incRef();
    }
    event = other.event;
  }
  
  TypedHandle<EvType>& operator=(const TypedHandle<EvType>& other)
  {
    // incref other first in case it is self asignment
    if(other.event)
    {
      other.event->incRef();
    }
    if(event)
    {
      event->decRef();
    }
    event = other.event;
    return *this;
  }

  
  virtual ~TypedHandle()
  {
    if(event)
    {
      event->decRef();
    }
  }
};

// there is only one event pool for the whole application
struct Pool
{
private:  
  tthread::mutex _mutex;

  struct SubPool
  {
  };

  template<typename EvType>
  struct TypedSubPool : public SubPool
  {
    typedef vector<EvType*> EventVector;
    EventVector events;
    
    uint32_t numEvents() {return events.size(); }

    // returns events with refcount 1
    EvType* createEvent(const event::Type& inType)
    {
      EvType* result = NULL;
      for(typename EventVector::iterator pos=events.begin(); 
          pos != events.end();
          ++pos)
      {
        if((*pos)->refcount == 0)
        {
          result = *pos;
//          std::cout << "reusing" << std::endl;
          break;
        }
      }
      if(!result)
      {
        result = new EvType(inType);
        events.push_back(result);
//        std::cout << "new, size now "<<events.size() << std::endl;
      }
      result->incRef();
      return result;
    }
  };
  
  typedef map<const char*, SubPool*> SubPoolMap;
  map<const char*, SubPool*> typeName2subPool;
  
  template<typename EvType>
  TypedSubPool<EvType>* findSubPool() // creates a subpool if necessary, always returns valid result
  {
    TypedSubPool<EvType>* result = NULL;
    const char* subpoolName = typeid(TypedHandle<EvType>).name();
    SubPoolMap::iterator pos = typeName2subPool.find(subpoolName);
    if(pos == typeName2subPool.end())
    {
      result = new TypedSubPool<EvType>();
      typeName2subPool[subpoolName] = result;
    }
    else {
      result = static_cast<TypedSubPool<EvType>*>(pos->second);
    }
    return result;
  }
public:
  Pool();
  ~Pool();
  
  static Pool* instance();
  
  template<typename EvType>
  TypedHandle<EvType> createEvent(const event::Type& inType)
  {
    tthread::lock_guard<tthread::mutex> lock(_mutex);
    TypedHandle<EvType> result;
    TypedSubPool<EvType>* sp = findSubPool<EvType>();
    EvType* ev = sp->createEvent(inType);
//    std::cout << "subpool " << typeid(TypedHandle<EvType>).name() << " has " << sp->numEvents() << " events" << std::endl;
    result.event = ev;
//    std::cout << "creating handle for type " << typeid(TypedHandle<EvType>).name() << std::endl;
    return result;
  }
};


}
}

#endif
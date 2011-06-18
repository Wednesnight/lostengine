#ifndef LOST_EVENT_POOL_H
#define LOST_EVENT_POOL_H

#include "lost/event/Event.h"
#include "lost/event/Handle.h"
#include "tinythread.h"

namespace lost
{
namespace event
{

struct Pool;

struct Handle
{
  Handle();
  virtual ~Handle();
  
  Pool* pool;
  Event* event;

};

template<typename EvType>
struct TypedHandle;

// there is only one event pool for the whole application
struct Pool
{
private:  
  tthread::mutex _mutex;
  struct SubPool
  {
    typedef map<Event*,int32_t> EventRefMap;
    EventRefMap event2refcount;
    EventRefMap unused;
    EventRefMap used;
    
    uint32_t numEvents() {return event2refcount.size(); }

    // returns events with refcount 1
    template<typename EvType>
    EvType* createEvent(const event::Type& inType)
    {
      EvType* result = NULL;
      EventRefMap::iterator pos = unused.begin();
      if(pos != unused.end())
      {
        result = static_cast<EvType*>(pos->first);
        result->type = inType;
      }
      else {
        result = new EvType(inType);
        used[result] = 1;
        event2refcount[result] = 1;
      }
      return result;
    }

    void incRef(Event* ev)
    {
      EventRefMap::iterator pos = event2refcount.find(ev);
      if(pos != event2refcount.end())
      {
        pos->second += 1;
      }
      else {
        std::cout << "WARNING: tried to increase refcount on event that is not managed in pool" << std::endl;
      }
    }

    void decRef(Event* ev)
    {
      EventRefMap::iterator pos = event2refcount.find(ev);
      if(pos != event2refcount.end())
      {
        if(pos->second > 0)
        {
          pos->second -= 1;
          if(pos->second == 0)
          {
            std::cout<<"moving event from used to unused" << std::endl;
            used.erase(pos->first);
            unused[pos->first] = 0;
          }
        }
        else
        {
          std::cout << "WARNING: tried to decrease refcount beyond zero" << std::endl;          
        }
      }
      else {
        std::cout << "WARNING: tried to decrease refcount on event that is not managed in pool" << std::endl;
      }
    }
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
public:
  Pool();
  ~Pool();
  
  static Pool* instance();
  
  template<typename EvType>
  TypedHandle<EvType> createEvent(const event::Type& inType)
  {
    tthread::lock_guard<tthread::mutex> lock(_mutex);
    TypedHandle<EvType> result;
    SubPool* sp = findSubPool<EvType>();
    EvType* ev = sp->createEvent<EvType>(inType);
    std::cout << "subpool " << typeid(TypedHandle<EvType>).name() << " has " << sp->numEvents() << " events" << std::endl;
    result.event = ev;
    result.pool = this;
    std::cout << "creating handle for type " << typeid(TypedHandle<EvType>).name() << std::endl;
    return result;
  }

  template<typename EvType>
  void incRef(Event* ev)
  {
    tthread::lock_guard<tthread::mutex> lock(_mutex);
    SubPool* sp = findSubPool<EvType>();
    sp->incRef(ev);
  }

  template<typename EvType>
  void decRef(Event* ev)
  {
    tthread::lock_guard<tthread::mutex> lock(_mutex);
    SubPool* sp = findSubPool<EvType>();
    sp->decRef(ev);
  }
  
    
};

template<typename EvType>
struct TypedHandle : public Handle
{
  EvType* operator->()
  {
    return static_cast<EvType*>(event);
  }

  TypedHandle()
  {
    pool = NULL;
    event = NULL;
  }
  
  TypedHandle(const TypedHandle<EvType>& other)
  {
    pool = other.pool;
    event = other.event;
    pool->incRef<EvType>(event);
  }
  
  TypedHandle<EvType>& operator=(const TypedHandle<EvType>& other)
  {
    // incref other first in case it is self asignment
    if(other.pool)
    {
      Pool* p = other.pool;
      Event* e = other.event;
      p->incRef<EvType>(e);
    }
    if(event && pool)
    {
      pool->decRef<EvType>(event);
    }
    pool = other.pool;
    event = other.event;
    return *this;
  }

  
  virtual ~TypedHandle()
  {
    if(pool)
    {
      pool->decRef<EvType>(event);
    }
  }
/*  EvType& operator*()
  {
    return *(static_cast<EvType*>(event));
  }*/
};

}
}

#endif
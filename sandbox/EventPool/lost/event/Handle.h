#ifndef LOST_EVENT_HANDLE_H
#define LOST_EVENT_HANDLE_H

namespace lost
{
namespace event
{

struct Pool;
struct Event;

struct Handle
{
  Handle();
  Handle(const Handle& other);
  virtual ~Handle();
  
  Pool* pool;
  Event* event;

};


template<typename EvType>
struct TypedHandle : public Handle
{
  EvType* operator->()
  {
    return static_cast<EvType*>(event);
  }
  
/*  EvType& operator*()
  {
    return *(static_cast<EvType*>(event));
  }*/
};

}
}

#endif
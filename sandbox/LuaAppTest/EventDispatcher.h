#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include "EventQueue.h"
#include <boost/function.hpp>
#include <map>
#include <iostream>
#include "lost/lua/lua.h"

// Asynchronous EventDispatcher
struct EventDispatcher
{
  typedef lost::shared_ptr<Event> EventPtr;
  typedef boost::function<void(EventPtr)> HandlerFuncType;
  typedef std::map<std::string, HandlerFuncType > HandlerMap;

  struct LuaHandlerFunc
  {
    luabind::object handler;
    LuaHandlerFunc(luabind::object inHandler) : handler(inHandler) {}
    void operator()(EventPtr event)
    {
      luabind::call_function<void>(handler, event);
    }
  };

  EventDispatcher() 
  {
  }

  virtual ~EventDispatcher() 
  {
  }
  
  // only queues events until the runloop decides it's time to really dispatch events 
  // and calls doDispatch
  void dispatchEvent(lost::shared_ptr<Event> event)
  {
    eventQueue.push(event);
  }

  // can only register one handler per event type for now
  // register C++ code her
  void addEventListener(const std::string& inEventType, HandlerFuncType handlerFunc)
  {
    type2handler[inEventType] = handlerFunc;
  }

  // register lua code her (this is really only for the lua binding
  void addEventListener(const std::string& inEventType, const luabind::object& handler)
  {
    std::cout << "adding lua listener" << std::endl;
    type2handler[inEventType] = LuaHandlerFunc(handler);
  }

  // call this to really dispatch events at defined intervalls
  void doDispatch()
  {
    while(eventQueue.size() > 0)
    {
      EventPtr event = eventQueue.pop();
      HandlerMap::iterator pos = type2handler.find(event->type);
      if(pos != type2handler.end())
      {
        pos->second(event);
      }
      else
      {
        std::cout << "discarding event of type: " << event->type << std::endl;
      }
    }
  }
  
private:
  HandlerMap type2handler;
  EventQueue  eventQueue;
};

#endif

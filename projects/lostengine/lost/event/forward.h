#ifndef LOST_EVENT_FORWARD_H
#define LOST_EVENT_FORWARD_H

namespace lost
{
namespace event
{

struct Event;
typedef lost::shared_ptr<Event> EventPtr;

struct EventDispatcher;
typedef lost::shared_ptr<EventDispatcher> EventDispatcherPtr;

}
}
#endif
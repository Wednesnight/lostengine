#ifndef LOST_EVENT_FORWARD_H
#define LOST_EVENT_FORWARD_H

namespace lost
{
namespace event
{

struct Pool;

struct Event;
typedef lost::shared_ptr<Event> EventPtr;

struct EventDispatcher;
typedef lost::shared_ptr<EventDispatcher> EventDispatcherPtr;

struct Listener;
typedef lost::shared_ptr<Listener> ListenerPtr;

struct Connection;
typedef lost::shared_ptr<Connection> ConnectionPtr;

struct Signal;
typedef lost::shared_ptr<Signal> SignalPtr;

}
}
#endif
#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include "Event.h"
#include <list>

// FIFO event queue
struct EventQueue
{
  std::size_t size() { return eventqueue.size(); }

  void push(lost::shared_ptr<Event> event){eventqueue.push_back(event); }
  lost::shared_ptr<Event> pop()
  {
    if(size() > 0)
    {
      lost::shared_ptr<Event> result = eventqueue.front();
      eventqueue.pop_front();
      return result;
    }
    else
      throw std::runtime_error("called pop on empty queue");
  }

private:
  std::list<lost::shared_ptr<Event> > eventqueue;
};

#endif

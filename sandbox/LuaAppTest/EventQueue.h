#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include <boost/shared_ptr.hpp>
#include "Event.h"
#include <list>

// FIFO event queue
struct EventQueue
{
  std::size_t size() { return eventqueue.size(); }

  void push(boost::shared_ptr<Event> event){eventqueue.push_back(event); }
  boost::shared_ptr<Event> pop()
  {
    if(size() > 0)
    {
      boost::shared_ptr<Event> result = eventqueue.front();
      eventqueue.pop_front();
      return result;
    }
    else
      throw std::runtime_error("called pop on empty queue");
  }

private:
  std::list<boost::shared_ptr<Event> > eventqueue;
};

#endif
#ifndef LOST_APPLICATION_QUEUEEVENT_H
#define LOST_APPLICATION_QUEUEEVENT_H

#include "lost/event/Event.h"
#include "lost/platform/shared_ptr.h"
#include "lost/resource/Loader.h"
#include "lost/common/Hash.h"

namespace lost
{
  namespace application
  {
    struct QueueEvent : public event::Event
    {
      event::EventPtr event;
      static const event::Type& QUEUE() { static const event::Type d = common::djb2Hash("queueEvent"); return d; }

      QueueEvent(const event::EventPtr& inEvent)
      : event(inEvent), Event(QUEUE())
      {
      }
      virtual ~QueueEvent() {}

    };

  }
}

#endif

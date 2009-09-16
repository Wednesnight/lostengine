#ifndef LOST_APPLICATION_PROCESSEVENT_H
#define LOST_APPLICATION_PROCESSEVENT_H

#include "lost/event/Event.h"
#include "lost/platform/shared_ptr.h"
#include "lost/resource/Loader.h"

namespace lost
{
  namespace application
  {

    struct ProcessEvent;
    typedef lost::shared_ptr<ProcessEvent> ProcessEventPtr;

    struct ProcessEvent : public event::Event
    {
        static const event::Type& PROCESS() { static const event::Type d = "processEvent"; return d; }

        ProcessEvent()
        : Event(PROCESS())
        {
        }
        virtual ~ProcessEvent() {}

    };

  }
}

#endif

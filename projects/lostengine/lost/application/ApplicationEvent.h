#ifndef LOST_APPLICATION_APPLICATIONEVENT_H
#define LOST_APPLICATION_APPLICATIONEVENT_H

#include "lost/event/Event.h"
#include "lost/common/Hash.h"

namespace lost
{
  namespace application
  {
    struct ApplicationEvent;
    typedef lost::shared_ptr<ApplicationEvent> ApplicationEventPtr;

    struct ApplicationEvent : public event::Event
    {
        static const event::Type& RUN()       { static event::Type d = common::djb2Hash("applicationRun");return d;}
        static const event::Type& QUIT()      { static event::Type d = common::djb2Hash("applicationQuit");return d;}

        ApplicationEvent(const event::Type& inType) : Event(inType) { }
        static ApplicationEventPtr create(const event::Type& inType) { return ApplicationEventPtr(new ApplicationEvent(inType)); }
        virtual ~ApplicationEvent() {}
    };
  }
}

#endif

#ifndef LOST_APPLICATION_APPLICATIONEVENT_H
#define LOST_APPLICATION_APPLICATIONEVENT_H

#include "lost/event/Event.h"

namespace lost
{
namespace application
{
struct ApplicationEvent : public event::Event
{
    static const event::Type& PREINIT() { static event::Type d = "applicationPreInit";return d;}
    static const event::Type& INIT()    { static event::Type d = "applicationInit";return d;}
    static const event::Type& POSTINIT(){ static event::Type d = "applicationPostInit";return d;}
    static const event::Type& RUN()     { static event::Type d = "applicationRun";return d;}
    static const event::Type& QUIT()    { static event::Type d = "applicationQuit";return d;}
    static const event::Type& IDLE()    { static event::Type d = "applicationIdle";return d;}

    ApplicationEvent(const event::Type& inType) : Event(inType) { }
    virtual ~ApplicationEvent() {}
};
}
}

#endif
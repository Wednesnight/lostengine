#ifndef LOST_APPLICATION_SPAWNTASKLETEVENT_H
#define LOST_APPLICATION_SPAWNTASKLETEVENT_H

#include "lost/event/Event.h"
#include "lost/resource/forward.h"

namespace lost
{
  namespace application
  {
    struct SpawnTaskletEvent : public event::Event
    {
      resource::LoaderPtr loader;
      static const event::Type& SPAWN_TASKLET() { static const event::Type d = "spawnTasklet"; return d; }

      SpawnTaskletEvent(resource::LoaderPtr inLoader)
      : loader(inLoader), Event(SPAWN_TASKLET())
      {
      }
        
      static SpawnTaskletEventPtr create(resource::LoaderPtr inLoader)
      {
        return SpawnTaskletEventPtr(new SpawnTaskletEvent(inLoader));
      }
        
      virtual ~SpawnTaskletEvent() {}

    };

  }
}

#endif

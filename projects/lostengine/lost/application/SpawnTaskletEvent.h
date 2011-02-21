#ifndef LOST_APPLICATION_SPAWNTASKLETEVENT_H
#define LOST_APPLICATION_SPAWNTASKLETEVENT_H

#include "lost/event/Event.h"
#include "lost/platform/shared_ptr.h"
#include "lost/resource/Loader.h"
#include "lost/resource/Writer.h"

namespace lost
{
  namespace application
  {

    struct SpawnTaskletEvent;
    typedef lost::shared_ptr<SpawnTaskletEvent> SpawnTaskletEventPtr;

    struct SpawnTaskletEvent : public event::Event
    {
      resource::LoaderPtr loader;
      resource::WriterPtr writer;
      static const event::Type& SPAWN_TASKLET() { static const event::Type d = "spawnTasklet"; return d; }

      SpawnTaskletEvent(resource::LoaderPtr inLoader, resource::WriterPtr inWriter)
      : loader(inLoader), writer(inWriter), Event(SPAWN_TASKLET())
      {
      }
        
      static SpawnTaskletEventPtr create(resource::LoaderPtr inLoader, resource::WriterPtr inWriter)
      {
        return SpawnTaskletEventPtr(new SpawnTaskletEvent(inLoader, inWriter));
      }
        
      virtual ~SpawnTaskletEvent() {}

    };

  }
}

#endif

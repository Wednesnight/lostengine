#ifndef LOST_ENGINE_ENGINE_H
#define LOST_ENGINE_ENGINE_H

#include "lost/common/Config.h"

namespace lost
{
  namespace engine
  {

    template<typename DerivedEngine,
             typename VideoSubSystem,
             typename AudioSubSystem,
             typename InputSubSystem,
             typename TimerSubSystem>
    struct Engine
    {
      InputSubSystem input;
      VideoSubSystem video;
      AudioSubSystem audio;
      TimerSubSystem timer;

      boost::shared_ptr<common::Config> config;

      Engine( boost::shared_ptr<common::Config> inConfig )
        : input(*(static_cast<DerivedEngine*>(this))),
          video(*(static_cast<DerivedEngine*>(this))),
          audio(*(static_cast<DerivedEngine*>(this))),
          timer(*(static_cast<DerivedEngine*>(this))),
          config(inConfig)
      {
      }

      virtual ~Engine() {}

      void init()
      {
        video.init();
        audio.init();
        input.init();
        timer.init();
      }

      virtual void run() = 0;
      virtual void shutdown() = 0;
    };

  }
}

#endif

#ifndef LOST_ENGINE_SDL_ENGINE_H
#define LOST_ENGINE_SDL_ENGINE_H

#include "lost/Engine.h"
#include "lost/sdl/VideoSubSystem.h"
#include "lost/sdl/AudioSubSystem.h"
#include "lost/sdl/InputSubSystem.h"
#include "lost/sdl/TimerSubSystem.h"

namespace lost
{
  namespace engine
  {
    namespace sdl
    {

      struct Engine;
      typedef lost::Engine<Engine,
                           sdl::VideoSubSystem<Engine>,
                           sdl::AudioSubSystem<Engine>,
                           sdl::InputSubSystem<Engine>,
                           sdl::TimerSubSystem<Engine> > BaseEngine;
      struct Engine : public BaseEngine
      {
        void init( int argc, char *argv[], const std::string& configFilename )
        {
          BaseEngine::init( argc, argv, configFilename );
        }
      };

    }
  }
}

#endif

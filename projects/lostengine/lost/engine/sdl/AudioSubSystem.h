#ifndef LOST_ENGINE_SDL_AUDIOSUBSYSTEM_H
#define LOST_ENGINE_SDL_AUDIOSUBSYSTEM_H

namespace lost
{
  namespace engine
  {
    namespace sdl
    {

      template<typename Engine>
      struct AudioSubSystem
      {
        Engine& engine;
        AudioSubSystem(Engine& inEngine) : engine(inEngine)
        {
        }

        void init()
        {
        }
      };

    }
  }
}

#endif

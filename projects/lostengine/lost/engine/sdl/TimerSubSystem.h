#ifndef LOST_ENGINE_SDL_TIMERSUBSYSTEM_H
#define LOST_ENGINE_SDL_TIMERSUBSYSTEM_H

namespace lost
{
  namespace engine
  {
    namespace sdl
    {

      template<typename Engine>
      struct TimerSubSystem
      {
        Engine& engine;
        TimerSubSystem(Engine& inEngine) : engine(inEngine)
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

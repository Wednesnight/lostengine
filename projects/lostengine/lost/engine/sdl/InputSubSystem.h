#ifndef LOST_ENGINE_SDL_INPUTSUBSYSTEM_H
#define LOST_ENGINE_SDL_INPUTSUBSYSTEM_H

namespace lost
{
  namespace engine
  {
    namespace sdl
    {

      template<typename Engine>
      struct InputSubSystem
      {
        Engine& engine;
        InputSubSystem(Engine& inEngine) : engine(inEngine)
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

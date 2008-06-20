#ifndef LOST_ENGINE_SDL_VIDEOSUBSYSTEM_H
#define LOST_ENGINE_SDL_VIDEOSUBSYSTEM_H

namespace lost
{
  namespace engine
  {
    namespace sdl
    {

      template<typename Engine>
      struct VideoSubSystem
      {
        Engine& engine;
        VideoSubSystem(Engine& inEngine) : engine(inEngine)
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

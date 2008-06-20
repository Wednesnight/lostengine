#ifndef LOST_ENGINE_NO_VIDEOSUBSYSTEM_H
#define LOST_ENGINE_NO_VIDEOSUBSYSTEM_H

namespace lost
{
  namespace engine
  {
    namespace no
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

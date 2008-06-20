#ifndef LOST_ENGINE_NO_AUDIOSUBSYSTEM_H
#define LOST_ENGINE_NO_AUDIOSUBSYSTEM_H

namespace lost
{
  namespace engine
  {
    namespace no
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

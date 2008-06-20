#ifndef LOST_ENGINE_NO_TIMERSUBSYSTEM_H
#define LOST_ENGINE_NO_TIMERSUBSYSTEM_H

namespace lost
{
  namespace engine
  {
    namespace no
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

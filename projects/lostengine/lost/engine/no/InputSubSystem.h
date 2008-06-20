#ifndef LOST_ENGINE_NO_INPUTSUBSYSTEM_H
#define LOST_ENGINE_NO_INPUTSUBSYSTEM_H

namespace lost
{
  namespace engine
  {
    namespace no
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

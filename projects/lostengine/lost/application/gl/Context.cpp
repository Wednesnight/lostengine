#include "lost/application/gl/Context.h"

namespace lost
{
  namespace application
  {
    namespace gl
    {

      void Context::pushState(const SharedState& newState)
      {
        stateStack.push_back(newState);
      }

      void Context::popState()
      {
        stateStack.pop_back();
      }

    }
  }
}

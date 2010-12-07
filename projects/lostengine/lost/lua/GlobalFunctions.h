#ifndef LOST_LUA_GLOBALFUNCTIONS_H
#define LOST_LUA_GLOBALFUNCTIONS_H

#include "lost/lua/forward.h"

namespace lost
{
  namespace lua
  {
    namespace GlobalFunctions
    {
      /** 
       * Populates global utility functions
       */
      void install(State& inState);
    }
  }
}

#endif

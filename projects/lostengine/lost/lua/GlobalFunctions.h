#ifndef LOST_LUA_GLOBALFUNCTIONS_H
#define LOST_LUA_GLOBALFUNCTIONS_H

#include "lost/forward/lost/lua"

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

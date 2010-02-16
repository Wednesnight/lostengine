#ifndef LOST_LUA_MODULELOADER_H
#define LOST_LUA_MODULELOADER_H

#include "lost/forward/lost/lua"

namespace lost
{
  namespace lua
  {
    namespace ModuleLoader
    {
      /** installs a custom module loader for the lua package system, that
       * uses the provided resourceLoader to locate the module source files.
       * NOTE: call this only after you've initialised your resource loader,
       * your state AND bound all lostengine classes to your state.
       */
      void install(State& inState);
    }
  }
}

#endif

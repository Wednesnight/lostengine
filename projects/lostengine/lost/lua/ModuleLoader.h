#ifndef LOST_LUA_MODULELOADER_H
#define LOST_LUA_MODULELOADER_H

#include "lost/forward/lost/lua/lua.h"
#include "lost/forward/lost/lua/State.h"
#include "lost/resource/Loader.h"

namespace lost
{
  namespace lua
  {
    namespace ModuleLoader
    {
      /** installs a custom module loader for the lua package system, thats
       * uses the provided resourceLoader to locate the module source files.
       * NOTE: call this only after you're initialised your resource loader,
       * your state AND bound all lostengine classes to your state.
       */
      void install(State& inState, boost::shared_ptr<resource::Loader> inLoader);
    }
  }
}

#endif

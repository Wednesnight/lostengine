#ifndef LOST_APPLICATION_TASKLETCONFIG_H
#define LOST_APPLICATION_TASKLETCONFIG_H

#include "lost/resource/forward.h"
#include "lost/lua/forward.h"
#include "lost/math/Rect.h"
#include <string>

namespace lost
{
  namespace application
  {

    /** loads a tasklets "config.lua" file with the given loader and parses its contents into
     * the member variables.
     * This class mainly exists to encapsualte the loading/parsing code and prevent the luabind data types
     * from leaking in other classes.
     */
    struct TaskletConfig
    {
      TaskletConfig();
      
      bool load(lua::StatePtr interpreter, resource::LoaderPtr loader);
      void reset();
        
      std::string   taskletName;
      bool          taskletWaitForEvents;
      bool          taskletHasWindow;
      
      std::string   windowTitle;
      math::Rect    windowRect;

      double framerate;
      
      bool          glVsync;
    };

  }
}

#endif

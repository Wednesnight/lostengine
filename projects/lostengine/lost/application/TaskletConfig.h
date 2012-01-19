/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef LOST_APPLICATION_TASKLETCONFIG_H
#define LOST_APPLICATION_TASKLETCONFIG_H

#include "lost/resource/forward.h"
#include "lost/lua/forward.h"
#include "lost/math/Rect.h"

namespace lost
{
  namespace application
  {

    /** loads a tasklets "config.lua" file with the given loader and parses its contents into
     * the member variables.
     * This class mainly exists to encapsualte the loading/parsing code and prevent the slub data types
     * from leaking in other classes.
     */
    struct TaskletConfig
    {
      TaskletConfig();
      
      bool load(lua::StatePtr interpreter, resource::LoaderPtr loader);
      void reset();
        
      string   taskletName;
      bool          taskletWaitForEvents;
      bool          taskletHasWindow;
      
      string   windowTitle;
      math::Rect    windowRect;

      double framerate;
      
      bool          glVsync;
      // FIXME:
      // multithreaded: toggle (probably only relevant for Mac)
      // rgba bit depths: bit depth per field or grouped as in RGBA_5551, RGBA_8888 ?
      // stencil buffer: toggle, accuracy
      // depth buffer
      // aa settings
      // iPhone UIView scaling factor
    };

  }
}

#endif

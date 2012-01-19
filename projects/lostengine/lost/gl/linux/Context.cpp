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

#include "lost/gl/Context.h"
#include "lost/gl/gl.h"

#include <boost/thread/tss.hpp>

namespace lost
{
  namespace gl
  {
    struct Context::ContextHiddenMembers
    {
      Display*    glDisplay;
      GLXDrawable glDrawable;
      GLXContext  glContext;
    };

    void context_cleanup(Context* p)
    {
    }
    // set to the current context in makeCurrent
    boost::thread_specific_ptr<Context> currentContext(context_cleanup);

    void Context::initialize()
    {
      hiddenMembers = new ContextHiddenMembers;
      hiddenMembers->glDisplay  = glXGetCurrentDisplay();
      hiddenMembers->glDrawable = glXGetCurrentDrawable();
      hiddenMembers->glContext  = glXGetCurrentContext();
      currentContext.reset(this);
    }

    void Context::finalize()
    {
      delete hiddenMembers;
    }

    void Context::makeCurrent()
    {
      if (glXGetCurrentDisplay() != hiddenMembers->glDisplay ||
          glXGetCurrentDrawable() != hiddenMembers->glDrawable ||
          glXGetCurrentContext() != hiddenMembers->glContext)
      {
        glXMakeCurrent(hiddenMembers->glDisplay, hiddenMembers->glDrawable, hiddenMembers->glContext);
        currentContext.reset(this);
      }
    }

    void Context::clearCurrent()
    {
      glXMakeCurrent(hiddenMembers->glDisplay, None, NULL);
    }
    
    void Context::swapBuffers()
    {
      glXSwapBuffers(hiddenMembers->glDisplay, hiddenMembers->glDrawable);
    }

    void Context::vsync(bool enable)
    {
      XSynchronize(hiddenMembers->glDisplay, enable ? True : False);
    }

    void Context::multithreaded(bool enable)
    {
      // FIXME: implement Context::multithreaded
    }

    void* Context::getCurrentOsSpecific()
    {
      return currentContext.get();
    }
    
    void Context::setCurrentOsSpecififc(void* ctx)
    {
      ((Context*) ctx)->makeCurrent();
    }

  }
}

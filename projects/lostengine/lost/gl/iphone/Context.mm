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
#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>


namespace lost
{
  namespace gl
  {
      

    struct Context::ContextHiddenMembers
    {
      EAGLContext* context;
    };
    
    void Context::initialize()
    {
      hiddenMembers = new ContextHiddenMembers;
      hiddenMembers->context = [EAGLContext currentContext];
    }

    void Context::finalize()
    {
      delete hiddenMembers;
    }

    void Context::makeCurrent()
    {
      [EAGLContext setCurrentContext: hiddenMembers->context];
    }

    void Context::swapBuffers()
    {
      [hiddenMembers->context presentRenderbuffer:GL_RENDERBUFFER];  
    }

    void* Context::getCurrentOsSpecific()
    {
      return [EAGLContext currentContext];
    }
    
    void Context::setCurrentOsSpecififc(void* ctx)
    {
      [EAGLContext setCurrentContext:((EAGLContext*)ctx)];
    }


    void Context::vsync(bool enable)
    {
      // FIXME: implement Context::vsync
    }
    
    void Context::multithreaded(bool enable)
    {
      // FIXME: implement Context::multithreaded
    }
    
  }
}

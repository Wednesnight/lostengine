/**
 * Note:
 *
 *   - application::gl::Context is designed to live within an application::Window
 *   - to reduce overhead the platform specific part expects that an OpenGL context already exists and is set as the thread's current context!
 *   - the ctor gets the current context and uses that handle for it's complete lifecycle (e.g. makeCurrent, swapBuffers)
 *
 * There is only one proper way to initialize an application::gl::Context:
 *   1.) create an application::Window (it'll create and activate an OpenGL context) or create an OpenGL context and make it the thread's current!
 *   2.) create the application::gl::Context
 *
 */

#ifndef LOST_APPLICATION_GL_CONTEXT_H
#define LOST_APPLICATION_GL_CONTEXT_H

#include <list>
#include "lost/gl/gl.h"
#include "lost/application/gl/State.h"

namespace lost
{
  namespace application
  {
    namespace gl
    {
    
      struct Context
      {
      private:
        /**
         * forward declaration for platform specific stuff
         */
        struct ContextHiddenMembers;
        ContextHiddenMembers* hiddenMembers;

        /**
         * hidden ctor/dtor utility methods for platform specific stuff
         */
        void initialize();
        void finalize();

        SharedState currentState;
        std::list<SharedState> stateStack;
        void apply(const SharedState& newState);
      public:
        Context();
        ~Context();

        /**
         * sets this OpenGL context as the thread's current context
         */
        void makeCurrent();

        /**
         * swaps buffers :)
         */
        void swapBuffers();

        void pushState(const SharedState& newState);
        void popState();
      };

    }
  }
}

#endif

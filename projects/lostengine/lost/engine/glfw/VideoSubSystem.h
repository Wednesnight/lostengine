#ifndef LOST_ENGINE_GLFW_VIDEOSUBSYSTEM_H
#define LOST_ENGINE_GLFW_VIDEOSUBSYSTEM_H

#include "GL/glfw.h"
#include "lost/common/DisplayAttributes.h"
#include <boost/signal.hpp>
#include <boost/bind.hpp>

namespace lost
{
  namespace engine
  {
    namespace glfw
    {
      typedef boost::signal<void ( int width, int height )> WindowSizeSignal;
      WindowSizeSignal windowSizeSignal;
      void glfwWindowSizeCallback(int width, int height)
      {
        windowSizeSignal(width, height);
      }

      template<typename Engine>
      struct VideoSubSystem
      {

        WindowSizeSignal        windowResize;
        Engine&                 engine;
        common::DisplayAttributes attributes;

        VideoSubSystem(Engine& inEngine) : engine(inEngine)
        {
        }

        void init()
        {
          windowSizeSignal.connect( windowResize );
        }

        void restart(const common::DisplayAttributes& attr)
        {
          // save used attributes to be able to read them later on
          attributes = attr;
          // FIXME: close window first if it was open before
          glfwOpenWindow(attributes.width,
                         attributes.height,
                         attributes.redbits,
                         attributes.greenbits,
                         attributes.bluebits,
                         attributes.alphabits,
                         attributes.depthbits,
                         attributes.stencilbits,
                         attributes.fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOW);
          glfwSetWindowTitle(attributes.title.c_str());
          glfwSetWindowSizeCallback(glfwWindowSizeCallback);
          engine.input.restart();
        }

        void swapBuffers()
        {
          glfwSwapBuffers();
        }

        void setWindowTitle(const std::string& inTitle)
        {
          glfwSetWindowTitle(inTitle.c_str());
        }
      };
    }
  }
}

#endif

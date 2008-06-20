#ifndef LOST_ENGINE_GLFW_ENGINE_H
#define LOST_ENGINE_GLFW_ENGINE_H

#include "lost/engine/Engine.h"
#include "lost/engine/glfw/VideoSubSystem.h"
#include "lost/engine/no/AudioSubSystem.h"
#include "lost/engine/glfw/InputSubSystem.h"
#include "lost/engine/glfw/TimerSubSystem.h"

#include "GL/glfw.h"

namespace lost
{
  namespace engine
  {
    namespace glfw
    {
      struct Engine;
      typedef lost::engine::Engine<Engine,
                                   glfw::VideoSubSystem<Engine>,
                                   no::AudioSubSystem<Engine>,
                                   glfw::InputSubSystem<Engine>,
                                   glfw::TimerSubSystem<Engine> > BaseEngine;
      struct Engine : public BaseEngine
      {
        bool running;

        Engine( boost::shared_ptr<common::Config> inConfig )
          : BaseEngine::Engine(inConfig),
            running(false)
        {
          glfwInit();
        }

        virtual ~Engine() { glfwTerminate(); }

        void init()
        {
          BaseEngine::init();
        }

        virtual void run()
        {
          running = true;
          while(running)
          {
            timer.update();
            running = running && glfwGetWindowParam(GLFW_OPENED);
            glfwSleep(timer.getTimerResolution());
          }
        }

        virtual void shutdown()
        {
          running = false;
        }
      };

    }
  }
}

#endif

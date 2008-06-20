#include "lost/application/Application.h"
#include <GL/glfw.h>
#include <iostream>
#include <algorithm>
#include "lost/engine/KeySym.h"
#include "lost/engine/MouseSym.h"
#include "lost/application/Timer.h"
#include <boost/bind.hpp>

using namespace std;
using namespace lost::engine;

namespace lost
{
namespace application
{
  Application* appInstance = NULL;

  void glfwKeyCallback( int keysym, int action )
  {
    appInstance->injectKey(keysym, action);
  }

  void glfwMouseMoveCallback( int x, int y )
  {
    appInstance->injectMouseMove(lost::math::Vec2(x, y));
  }

  void glfwMouseButtonCallback( int button, int action )
  {
    int x, y;
    glfwGetMousePos( &x, &y );
    appInstance->injectMouseButton(button, (action == GLFW_PRESS), lost::math::Vec2(x, y) );
  }

  void glfwWindowSizeCallback(int width, int height)
  {
    appInstance->injectWindowResize(width, height);
  }


  Application::Application() { init(); }
  Application::Application(common::Config& config) : displayAttributes(config) { init(); }
  Application::Application(const common::DisplayAttributes& attribs) :  displayAttributes(attribs) { init(); }
  Application::~Application() { glfwTerminate(); }

  void Application::init()
  {
    if(appInstance != NULL)
    {
      throw std::runtime_error("appInstance wasn'nt NULL, did you instance more than one app?");
    }
    appInstance = this;
    glfwInit();    // must init glfw before anything else
    initDisplay(); // must be initialised first or no input callbacks will be called
    initCallbacks(); // connect callbacks AFTER display was initialised or nothing will happen (yes, I know, I'm repeating myslef but you don't wanna know how much time we lost tracking stuff like this down)
    running = false;
    // connect resize callback to ourselves so we can keep track of the window size in the displayAttributes
    windowResize.connect(boost::bind(&Application::resize, this, _1, _2));
  }

  void Application::resize(int width, int height)
  {
    displayAttributes.width = width;
    displayAttributes.height = height;
  }

  void Application::injectMouseMove(const lost::math::Vec2& pos)
  {
    lost::math::Vec2 point = pos;
    lost::event::MouseEvent event;
    point.y = displayAttributes.height - point.y;
    event.type = lost::event::MouseMoveEventType;
    event.pos = point;
    event.button = M_UNKNOWN;
    event.pressed = false;
    mouseMove( event );
  }

  void Application::injectMouseButton(int button, bool pressed, const lost::math::Vec2& pos)
  {
    lost::event::MouseEvent event;
    lost::math::Vec2 point = pos;
    point.y = displayAttributes.height - point.y;
    event.type = lost::event::MouseButtonEventType;
    event.pos = point;
    event.button = (button == GLFW_MOUSE_BUTTON_1)
                     ? M_LEFT
                     : ((button == GLFW_MOUSE_BUTTON_2)
                         ? M_RIGHT
                         : ((button == GLFW_MOUSE_BUTTON_3)
                             ? M_MIDDLE
                             : M_UNKNOWN));
    event.pressed = pressed;
    mouseButton( event );
  }

  void Application::injectKey(int keysym, int pressed)
  {
    int l_key = keysym;
    if (keysym > GLFW_KEY_SPECIAL && keysym <= GLFW_KEY_LAST)
    {
      if (keysym >= GLFW_KEY_F1 && keysym <= GLFW_KEY_F12)
      {
        l_key = K_F1 + (keysym - GLFW_KEY_F1);
      }
      else
      {
        switch (keysym)
        {
          case GLFW_KEY_TAB :
            l_key = K_TAB;
            break;
          case GLFW_KEY_LSHIFT :
            l_key = K_LSHIFT;
            break;
          case GLFW_KEY_RSHIFT :
            l_key = K_RSHIFT;
            break;
          case GLFW_KEY_LCTRL :
            l_key = K_LCTRL;
            break;
          case GLFW_KEY_RCTRL :
            l_key = K_RCTRL;
            break;
          case GLFW_KEY_BACKSPACE :
            l_key = K_BACKSPACE;
            break;
          case GLFW_KEY_ENTER :
            l_key = K_ENTER;
            break;
          case GLFW_KEY_HOME :
            l_key = K_HOME;
            break;
          case GLFW_KEY_END :
            l_key = K_END;
            break;
          case GLFW_KEY_DEL :
            l_key = K_DELETE;
            break;
          case GLFW_KEY_INSERT :
            l_key = K_INSERT;
            break;
          case GLFW_KEY_PAGEUP :
            l_key = K_PAGEUP;
            break;
          case GLFW_KEY_PAGEDOWN :
            l_key = K_PAGEDOWN;
            break;
          case GLFW_KEY_UP :
            l_key = K_UP;
            break;
          case GLFW_KEY_DOWN :
            l_key = K_DOWN;
            break;
          case GLFW_KEY_LEFT :
            l_key = K_LEFT;
            break;
          case GLFW_KEY_RIGHT :
            l_key = K_RIGHT;
            break;
          case GLFW_KEY_ESC :
            l_key = K_ESCAPE;
            break;
          default :
            l_key = keysym;
        }
      }
    }
    lost::event::KeyEvent event;
    event.key = l_key;
    event.pressed = pressed;
    key(event);
  }

  void Application::injectWindowResize(int x, int y)
  {
    windowResize(x, y);
  }

  void Application::initDisplay()
  {
      int result = glfwOpenWindow(displayAttributes.width,
                     displayAttributes.height,
                     displayAttributes.redbits,
                     displayAttributes.greenbits,
                     displayAttributes.bluebits,
                     displayAttributes.alphabits,
                     displayAttributes.depthbits,
                     displayAttributes.stencilbits,
                     displayAttributes.fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOW);
      if(!result)
      {
        throw std::runtime_error("couldn't open display");
      }
      glfwSetWindowTitle(displayAttributes.title.c_str());
      glfwSetWindowSizeCallback(glfwWindowSizeCallback);
  }

  void Application::initCallbacks()
  {
    glfwEnable( GLFW_KEY_REPEAT );
    glfwSetKeyCallback( glfwKeyCallback );
    glfwSetMousePosCallback( glfwMouseMoveCallback );
    glfwSetMouseButtonCallback( glfwMouseButtonCallback );
  }

  void Application::run()
  {
    running = true;
    double lastTime = glfwGetTime();
    while(running)
    {
      double now = glfwGetTime();
      double delta = now - lastTime;
      lastTime = now;
      updateTimers(delta);
      glfwPollEvents();
      glfwSleep(.001); // stupid fucking hack because glfw doesn't support OS timers and we need to measure manually.
      if(!glfwGetWindowParam(GLFW_OPENED))
      {
        running = false;
      }
    }
  }

  void Application::updateTimers(double deltaSec)
  {
    for(list<Timer*>::iterator i = timers.begin(); i!=timers.end(); ++i)
    {
      (*i)->update(deltaSec);
    }
  }

  void Application::quit() { running=false; }

  void Application::addTimer(Timer* timer)
  {
    timers.push_back(timer);
  }

  void Application::removeTimer(Timer* timer)
  {
    list<Timer*>::iterator pos = find(timers.begin(), timers.end(), timer);
    if(pos != timers.end())
    {
      timers.erase(pos);
    }
    else
    {
      WOUT("tried to remove timer "<<timer->name() << " which wasn't previously added");
    }
  };
}
}

#include "lost/application/glfw/GlfwAdapter.h"
#include "lost/common/Logger.h"
#include <GL/glfw.h>
#include "lost/application/KeySym.h"
#include "lost/application/MouseSym.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/MouseEvent.h"
#include "lost/application/ResizeEvent.h"

using namespace std;
using namespace boost;

namespace lost
{
namespace application
{

  GlfwAdapter* glfwAdapterInstance = NULL;

  GlfwAdapter::GlfwAdapter(event::EventDispatcher* inTarget)
  : target(inTarget)
  {
    if(glfwAdapterInstance != NULL)
    {
      throw std::runtime_error("glfwAdapterInstance wasn't NULL, did you instance more than one adapter/application?");
    }
    glfwAdapterInstance = this;
    running = false;// the main loop is not running yet
  }
  
  GlfwAdapter::~GlfwAdapter()
  {
  }

  void GlfwAdapter::init(const common::DisplayAttributes& displayAttributes)
  {
    glfwInit();    // must init glfw before anything else
    initDisplay(displayAttributes); // must be initialised first or no input callbacks will be called
    initCallbacks(); // connect callbacks AFTER display was initialised or nothing will happen (yes, I know, I'm repeating myslef but you don't wanna know how much time we lost tracking stuff like this down)  
  }

  void GlfwAdapter::run()
  {
    running = true;
    double lastTime = glfwGetTime();
    while(running)
    {
      double now = glfwGetTime();
      double delta = now - lastTime;
      lastTime = now;
      timerManager.updateTimers(delta);
      glfwPollEvents();
      glfwSleep(.001); // don't eat CPU time with a pure spin wait, sleep in intervals // FIXME: make this configurable?
      if(!glfwGetWindowParam(GLFW_OPENED))
      {
        running = false;
      }
    }  
  }

  void GlfwAdapter::quit()
  {
    running = false;
  }

  void GlfwAdapter::initDisplay(const common::DisplayAttributes& displayAttributes)
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
      displayHeight = displayAttributes.height;
  }

  void GlfwAdapter::initCallbacks()
  {
    glfwEnable(GLFW_KEY_REPEAT);
    glfwSetKeyCallback( glfwKeyCallback );
    glfwSetMousePosCallback( glfwMouseMoveCallback );
    glfwSetMouseButtonCallback( glfwMouseButtonCallback );
    glfwSetWindowSizeCallback(glfwWindowSizeCallback);
  }

  void glfwKeyCallback( int keysym, int action, int repeat )
  {
    glfwAdapterInstance->injectKey(keysym, action, repeat);
  }

  void glfwMouseMoveCallback( int x, int y )
  {
    glfwAdapterInstance->injectMouseMove(lost::math::Vec2(x, y));
  }

  void glfwMouseButtonCallback( int button, int action )
  {
    int x, y;
    glfwGetMousePos( &x, &y );
    glfwAdapterInstance->injectMouseButton(button, (action == GLFW_PRESS), lost::math::Vec2(x, y) );
  }

  void glfwWindowSizeCallback(int width, int height)
  {
    glfwAdapterInstance->injectWindowResize(width, height);
  }


  void GlfwAdapter::injectMouseMove(const lost::math::Vec2& pos)
  {
    lost::math::Vec2 point = pos;
        
    shared_ptr<MouseEvent> ev(new MouseEvent(MouseEvent::MOUSE_MOVE()));
    point.y = displayHeight - point.y;
    ev->pos = point;
    ev->button = M_UNKNOWN;
    ev->pressed = false;
    target->dispatchEvent(ev);
  }

  void GlfwAdapter::injectMouseButton(int button, bool pressed, const lost::math::Vec2& pos)
  {
    shared_ptr<MouseEvent> ev(new MouseEvent(pressed ? MouseEvent::MOUSE_DOWN() : MouseEvent::MOUSE_UP()));
    lost::math::Vec2 point = pos;
    point.y = displayHeight - point.y;

    ev->pos = point;
    ev->button = (button == GLFW_MOUSE_BUTTON_1)
                     ? M_LEFT
                     : ((button == GLFW_MOUSE_BUTTON_2)
                         ? M_RIGHT
                         : ((button == GLFW_MOUSE_BUTTON_3)
                             ? M_MIDDLE
                             : M_UNKNOWN));
    ev->pressed = pressed;
    target->dispatchEvent(ev);
  }

  void GlfwAdapter::injectKey(int keysym, int pressed, int repeat)
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
          case GLFW_KEY_TAB:l_key=K_TAB;break;
          case GLFW_KEY_LSHIFT:l_key=K_LSHIFT;break;
          case GLFW_KEY_RSHIFT:l_key=K_RSHIFT;break;
          case GLFW_KEY_LCTRL:l_key=K_LCTRL;break;
          case GLFW_KEY_RCTRL:l_key=K_RCTRL;break;
          case GLFW_KEY_BACKSPACE:l_key=K_BACKSPACE;break;
          case GLFW_KEY_ENTER:l_key=K_ENTER;break;
          case GLFW_KEY_HOME:l_key=K_HOME;break;
          case GLFW_KEY_END:l_key=K_END;break;
          case GLFW_KEY_DEL:l_key=K_DELETE;break;
          case GLFW_KEY_INSERT:l_key=K_INSERT;break;
          case GLFW_KEY_PAGEUP:l_key=K_PAGEUP;break;
          case GLFW_KEY_PAGEDOWN:l_key=K_PAGEDOWN;break;
          case GLFW_KEY_UP:l_key=K_UP;break;
          case GLFW_KEY_DOWN:l_key=K_DOWN;break;
          case GLFW_KEY_LEFT:l_key=K_LEFT;break;
          case GLFW_KEY_RIGHT:l_key=K_RIGHT;break;
          case GLFW_KEY_ESC:l_key=K_ESCAPE;break;
          default:l_key=keysym;
        }
      }
    }
        
    shared_ptr<KeyEvent> ev(new KeyEvent(pressed ? KeyEvent::KEY_DOWN() : KeyEvent::KEY_UP()));
    ev->key = l_key;
    ev->pressed = pressed;
    ev->repeat = repeat;
    target->dispatchEvent(ev);
  }

  void GlfwAdapter::injectWindowResize(int x, int y)
  {
    displayHeight = y;
    shared_ptr<ResizeEvent> ev(new ResizeEvent(ResizeEvent::MAIN_WINDOW_RESIZE()));
    ev->width = x;
    ev->height = y;
    target->dispatchEvent(ev);
  }
  
  void GlfwAdapter::terminate()
  {
    glfwTerminate();
  }

  void GlfwAdapter::sleep(double seconds)
  {
    glfwSleep(seconds);
  }
}
}
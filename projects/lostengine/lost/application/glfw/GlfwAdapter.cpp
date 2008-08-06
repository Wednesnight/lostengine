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

  // there is only one global adapter instance, but it's not public
  GlfwAdapter* glfwAdapterInstance = NULL;

  // prototypes for callbacks
  void glfwWindowSizeCallback(int width, int height);
  void glfwMouseMoveCallback( int x, int y );
  void glfwMouseButtonCallback( int button, int action );
  void glfwKeyCallback( int key, int action, int repeat );

  
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

    // connect callbacks AFTER display was initialised or nothing will happen (yes, I know, I'm repeating myslef but you don't wanna know how much time we lost tracking stuff like this down)  
    glfwEnable(GLFW_KEY_REPEAT);
    glfwSetKeyCallback( glfwKeyCallback );
    glfwSetMousePosCallback( glfwMouseMoveCallback );
    glfwSetMouseButtonCallback( glfwMouseButtonCallback );
    glfwSetWindowSizeCallback(glfwWindowSizeCallback);    
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

  void glfwKeyCallback( int keysym, int pressed, int repeat )
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
    glfwAdapterInstance->target->dispatchEvent(ev);    
  }

  void glfwMouseMoveCallback( int x, int y )
  {
    lost::math::Vec2 point(x,y);    
    shared_ptr<MouseEvent> ev(new MouseEvent(MouseEvent::MOUSE_MOVE()));
    point.y = glfwAdapterInstance->displayHeight - point.y;
    ev->pos = point;
    ev->button = M_UNKNOWN;
    ev->pressed = false;
    glfwAdapterInstance->target->dispatchEvent(ev);
  }

  void glfwMouseButtonCallback( int button, int action )
  {
    int x, y;
    glfwGetMousePos( &x, &y );
    //glfwAdapterInstance->injectMouseButton(button, (action == GLFW_PRESS), lost::math::Vec2(x, y) );
    
    bool pressed = (action == GLFW_PRESS);
    shared_ptr<MouseEvent> ev(new MouseEvent(pressed ? MouseEvent::MOUSE_DOWN() : MouseEvent::MOUSE_UP()));
    lost::math::Vec2 point(x,y);
    point.y = glfwAdapterInstance->displayHeight - point.y;
    ev->pos = point;
    ev->button = (button == GLFW_MOUSE_BUTTON_1)
    ? M_LEFT
    : ((button == GLFW_MOUSE_BUTTON_2)
       ? M_RIGHT
       : ((button == GLFW_MOUSE_BUTTON_3)
          ? M_MIDDLE
          : M_UNKNOWN));
    ev->pressed = pressed;
    glfwAdapterInstance->target->dispatchEvent(ev);
  }

  void glfwWindowSizeCallback(int width, int height)
  {
    glfwAdapterInstance->displayHeight = height;
    shared_ptr<ResizeEvent> ev(new ResizeEvent(ResizeEvent::MAIN_WINDOW_RESIZE()));
    ev->width = width;
    ev->height = height;
    glfwAdapterInstance->target->dispatchEvent(ev);    
  }

  void GlfwAdapter::terminate()
  {
    glfwTerminate();
  }
}
}
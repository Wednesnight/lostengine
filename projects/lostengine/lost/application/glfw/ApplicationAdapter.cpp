#include "lost/application/glfw/ApplicationAdapter.h"
#include "lost/common/Logger.h"
#include <GL/glfw.h>
#include "lost/application/KeySym.h"
#include "lost/application/MouseSym.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/MouseEvent.h"
#include "lost/application/ResizeEvent.h"
#include "lost/application/glfw/TimerManager.h"
#include "lost/math/Vec2.h"

using namespace std;
using namespace boost;
using namespace lost;
using namespace lost::event;
using namespace lost::common;
using namespace lost::application;


struct ApplicationAdapterState
{
  int                               displayHeight;   
  lost::application::TimerManager   timerManager;  
  bool                              running;
};

// there is only one global adapter instance, but it's not public
ApplicationAdapter* adapterInstance = NULL;

// prototypes for callbacks
void glfwWindowSizeCallback(int width, int height);
void glfwMouseMoveCallback( int x, int y );
void glfwMouseButtonCallback( int button, int action );
void glfwKeyCallback( int key, int action, int repeat );


ApplicationAdapter::ApplicationAdapter(EventDispatcher* inTarget)
: target(inTarget)
{
  if(adapterInstance != NULL)
  {
    throw std::runtime_error("glfwAdapterInstance wasn't NULL, did you instance more than one adapter/application?");
  }
  adapterInstance = this;
  state.reset(new ApplicationAdapterState);
  state->running = false;// the main loop is not running yet
}

ApplicationAdapter::~ApplicationAdapter()
{
}

void ApplicationAdapter::init(const DisplayAttributes& displayAttributes)
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
  state->displayHeight = displayAttributes.height;

  // connect callbacks AFTER display was initialised or nothing will happen (yes, I know, I'm repeating myslef but you don't wanna know how much time we lost tracking stuff like this down)  
  glfwEnable(GLFW_KEY_REPEAT);
  glfwSetKeyCallback( glfwKeyCallback );
  glfwSetMousePosCallback( glfwMouseMoveCallback );
  glfwSetMouseButtonCallback( glfwMouseButtonCallback );
  glfwSetWindowSizeCallback(glfwWindowSizeCallback);    
}

void ApplicationAdapter::run()
{
  state->running = true;
  double lastTime = glfwGetTime();
  while(state->running)
  {
    double now = glfwGetTime();
    double delta = now - lastTime;
    lastTime = now;
    state->timerManager.updateTimers(delta);
    glfwPollEvents();
    glfwSleep(.001); // don't eat CPU time with a pure spin wait, sleep in intervals // FIXME: make this configurable?
    if(!glfwGetWindowParam(GLFW_OPENED))
    {
      state->running = false;
    }
  }  
}

void ApplicationAdapter::quit()
{
  state->running = false;
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
  adapterInstance->target->dispatchEvent(ev);    
}

void glfwMouseMoveCallback( int x, int y )
{
  lost::math::Vec2 point(x,y);    
  shared_ptr<MouseEvent> ev(new MouseEvent(MouseEvent::MOUSE_MOVE()));
  point.y = adapterInstance->state->displayHeight - point.y;
  ev->pos = point;
  ev->button = M_UNKNOWN;
  ev->pressed = false;
  adapterInstance->target->dispatchEvent(ev);
}

void glfwMouseButtonCallback( int button, int action )
{
  int x, y;
  glfwGetMousePos( &x, &y );
  //glfwAdapterInstance->injectMouseButton(button, (action == GLFW_PRESS), lost::math::Vec2(x, y) );
  
  bool pressed = (action == GLFW_PRESS);
  shared_ptr<MouseEvent> ev(new MouseEvent(pressed ? MouseEvent::MOUSE_DOWN() : MouseEvent::MOUSE_UP()));
  lost::math::Vec2 point(x,y);
  point.y = adapterInstance->state->displayHeight - point.y;
  ev->pos = point;
  ev->button = (button == GLFW_MOUSE_BUTTON_1)
  ? M_LEFT
  : ((button == GLFW_MOUSE_BUTTON_2)
     ? M_RIGHT
     : ((button == GLFW_MOUSE_BUTTON_3)
        ? M_MIDDLE
        : M_UNKNOWN));
  ev->pressed = pressed;
  adapterInstance->target->dispatchEvent(ev);
}

void glfwWindowSizeCallback(int width, int height)
{
  adapterInstance->state->displayHeight = height;
  shared_ptr<ResizeEvent> ev(new ResizeEvent(ResizeEvent::MAIN_WINDOW_RESIZE()));
  ev->width = width;
  ev->height = height;
  adapterInstance->target->dispatchEvent(ev);    
}

void ApplicationAdapter::swapBuffers()
{
  glfwSwapBuffers();
}

void ApplicationAdapter::terminate()
{
  glfwTerminate();
}

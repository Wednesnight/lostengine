#include "Controller.h"
#include "lost/application/Application.h"

using namespace std;
using namespace lost::gl;
using namespace lost::gl::utils;
using namespace lost::math;
using namespace lost::common;
using namespace lost::event;
using namespace lost::application;
using namespace lost::platform;
using namespace lost::bitmap;
using namespace lost::resource;
using namespace lost::application;
using namespace boost;

Controller::Controller()
: traceMouse(false)
{
}

void Controller::redraw(shared_ptr<TimerEvent> event)
{
  shared_ptr<State> newState = appInstance->context->copyState();
  newState->vertexArray = true;
  appInstance->context->pushState(newState);

  set2DProjection(Vec2(0,0), Vec2(appInstance->displayAttributes.width, appInstance->displayAttributes.height));
  glClearColor(0,0,0,0);GLDEBUG;
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );GLDEBUG;
  
  glMatrixMode(GL_MODELVIEW);GLDEBUG;
  glLoadIdentity();GLDEBUG;

  setColor(redColor);
  for (vector<Vec2>::iterator idx = mouseCoords.begin(); idx != mouseCoords.end(); ++idx)
  {
    drawPoint(*idx);
  }

  fpsMeter.render(2,2,event->passedSec);

  appInstance->context->popState();
  appInstance->swapBuffers();
}

void Controller::preinit(shared_ptr<ApplicationEvent> event)
{
  appInstance->addEventListener(ApplicationEvent::INIT(), receive<ApplicationEvent>(bind(&Controller::init, this, _1)));
  appInstance->addEventListener(ResizeEvent::MAIN_WINDOW_RESIZE(), receive<ResizeEvent>(bind(&Controller::resize, this, _1)));
  appInstance->addEventListener(MouseEvent::MOUSE_DOWN(), receive<MouseEvent>(bind(&Controller::handleClick, this, _1)));
  appInstance->addEventListener(MouseEvent::MOUSE_MOVE(), receive<MouseEvent>(bind(&Controller::handleMove, this, _1)));
  appInstance->addEventListener(MouseEvent::MOUSE_UP(), receive<MouseEvent>(bind(&Controller::handleClick, this, _1)));
}

void Controller::init(shared_ptr<ApplicationEvent> event)
{
  redrawTimer.reset(new Timer("redrawTimer", 1.0/60.0));
  redrawTimer->addEventListener(TimerEvent::TIMER_FIRED(), receive<TimerEvent>(bind(&Controller::redraw, this, _1)));
}

void Controller::resize(shared_ptr<ResizeEvent> event)
{
  glViewport(0, 0, event->width, event->height);GLDEBUG;
}

void Controller::handleClick(shared_ptr<MouseEvent> event)
{
  traceMouse = event->pressed && event->button == MB_LEFT;
}

void Controller::handleMove(shared_ptr<MouseEvent> event)
{
  if (traceMouse)
  {
    mouseCoords.push_back(event->pos);
  }
}

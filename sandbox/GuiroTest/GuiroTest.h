#ifndef GUIROTEST_H
#define GUIROTEST_H

#include <boost/shared_ptr.hpp>

#include "lost/application/Application.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/KeySym.h"
#include "lost/application/MouseEvent.h"
#include "lost/application/Timer.h"
#include "lost/common/FpsMeter.h"
#include "lost/common/Logger.h"
#include "lost/event/Event.h"
#include "lost/gl/Draw.h"
#include "lost/gl/Utils.h"
#include "lost/lua/LuaBindings.h"
#include "lost/math/Vec2.h"

using namespace boost;
using namespace lost::application;
using namespace lost::common;
using namespace lost::event;
using namespace lost::gl;
using namespace lost::gl::utils;
using namespace lost::lua;
using namespace lost::math;
using namespace luabind;
using namespace std;

struct View
{
  View()
  {
    DOUT("View::View()");
  }
  
  virtual ~View()
  {
    DOUT("View::~View()");
  }

  virtual void internalRender()
  {
    render();
  }

  virtual void render()
  {
    DOUT("View::render()");
  }
};

struct ViewLuaWrapper : View, luabind::wrap_base
{
  ViewLuaWrapper()
  : View()
  {
  }
  
  virtual void render()
  {
    call<void>("render");
  }
  
  static void renderBase(View* base)
  {
    return base->View::render();
  }

  virtual void internalRender()
  {
    call<void>("internalRender");
  }
  
  static void internalRenderBase(View* base)
  {
    return base->View::internalRender();
  }
};

LOST_LUA_EXPORT_BEGIN(GuiroTestView)
{
  module(state, "lost")
  [
    namespace_("guiro")
    [
      class_<View, ViewLuaWrapper>("View")
      .def(constructor<>())
      .def("render", &View::render, &ViewLuaWrapper::renderBase)
      .def("internalRender", &View::internalRender, &ViewLuaWrapper::internalRenderBase)
    ],
    namespace_("gl")
    [
      def("setColor", &setColor),
      def("drawRectFilled", &drawRectFilled)
    ]
  ];
}
LOST_LUA_EXPORT_END

struct GuiroTest
{
  Timer                     renderTimer;
  boost::signal<void(void)> quit;
  FpsMeter                  fpsMeter;
  
  GuiroTest(Application& app)
  : renderTimer("render", 0.015)
  {
    GuiroTestView(*(app.interpreter));
    
    app.addEventListener(ApplicationEvent::PREINIT(), receive<Event>(bind(&GuiroTest::preinit, this, _1)));
  }
  
  void preinit(shared_ptr<Event> event)
  {
    appInstance->addEventListener(ApplicationEvent::INIT(), receive<Event>(bind(&GuiroTest::init, this, _1)));
  }
  
  void init(shared_ptr<Event> event)
  {
    DOUT("init()");

    appInstance->addEventListener(KeyEvent::KEY_UP(), receive<KeyEvent>(bind(&GuiroTest::keyHandler, this, _1)));
    appInstance->addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&GuiroTest::keyHandler, this, _1)));
    appInstance->addEventListener(MouseEvent::MOUSE_UP(), receive<MouseEvent>(bind(&GuiroTest::mouseClickHandler, this, _1)));
    appInstance->addEventListener(MouseEvent::MOUSE_DOWN(), receive<MouseEvent>(bind(&GuiroTest::mouseClickHandler, this, _1)));
    appInstance->addEventListener(MouseEvent::MOUSE_MOVE(), receive<MouseEvent>(bind(&GuiroTest::mouseMoveHandler, this, _1)));
    appInstance->addEventListener(ResizeEvent::MAIN_WINDOW_RESIZE(), receive<ResizeEvent>(bind(&GuiroTest::resizeHandler, this, _1)));
    quit.connect(bind(&Application::quit, appInstance));
    
    renderTimer.addEventListener(TimerEvent::TIMER_FIRED(), receive<TimerEvent>(bind(&GuiroTest::render, this, _1)));
  }
  
  void render(shared_ptr<TimerEvent> event)
  {
    glClearColor(0.0, 0.0, 0.0, 0.0);GLDEBUG;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);GLDEBUG;

    set2DProjection(Vec2(0,0), Vec2(appInstance->displayAttributes.width, appInstance->displayAttributes.height));
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnableClientState(GL_VERTEX_ARRAY);GLDEBUG;
    glDisable(GL_DEPTH_TEST);GLDEBUG;
    glDisable(GL_TEXTURE_2D);GLDEBUG;
    fpsMeter.render(appInstance->displayAttributes.width - fpsMeter.width, 0, event->passedSec);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    appInstance->swapBuffers();
  }
  
  void keyHandler(shared_ptr<KeyEvent> event)
  {
    DOUT("key: " << event->key);
    if (event->pressed)
    {
      switch (event->key)
      {
        case K_ESCAPE:
          quit();
          break;
      }
    }
  }

  void mouseClickHandler(shared_ptr<MouseEvent> event)
  {
  }
  
  void mouseMoveHandler(shared_ptr<MouseEvent> event)
  {
  }
  
  void resizeHandler(shared_ptr<ResizeEvent> event)
  {
    glViewport (0, 0, event->width, event->height); 
  }

};

#endif

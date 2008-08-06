#include "lost/common/Logger.h"
#include "lost/application/Application.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/application/TimerEvent.h"
#include "lost/gl/Utils.h"
#include "lost/gl/Draw.h"
#include "lost/math/Vec2.h"
#include "lost/application/Timer.h"
//#include "lost/common/FpsMeter.h"

using namespace std;
using namespace boost;
using namespace lost::common;
using namespace lost::event;
using namespace lost::math;
using namespace lost::gl;
using namespace lost::gl::utils;
using namespace lost::application;


//FpsMeter fpsMeter;

void idle(shared_ptr<TimerEvent> event)
{
  DOUT("idle");
  int width = appInstance->displayAttributes.width;
  int height = appInstance->displayAttributes.height;
  
  glViewport(0, 0, width, height);GLDEBUG;
  set2DProjection(Vec2(0, 0), Vec2(width, height));GLDEBUG;

  glDisable(GL_DEPTH_TEST);GLDEBUG;
  glDisable(GL_TEXTURE_2D);GLDEBUG;
  glClearColor( 0.0, 0.0, 0.0, 0.0 );GLDEBUG;
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );GLDEBUG;

  setColor(whiteColor);
  drawLine(Vec2(0,0), Vec2(width-1, height-1));
    
  //fpsMeter.render(200,0,event->passedSec);
  appInstance->glc.swapBuffers();
}

int main(int argn, char** args)
{
  LogLevel( log_all );
  try
  {
    DOUT("starting up");
    Application app;
    Timer timer("redraw", 1.0/60.0);
    timer.addEventListener(TimerEvent::TIMER_FIRED(), receive<TimerEvent>(idle));
    app.run();
    DOUT("shutting down");
  }
  catch (exception& e)
  {
    EOUT("exception: " << e.what());
  }

  return 0;
}
#include "lost/common/Logger.h"
#include "lost/application/Application.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/application/TimerEvent.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/KeySym.h"
#include "lost/gl/Utils.h"
#include "lost/gl/Draw.h"
#include "lost/math/Vec2.h"
#include "lost/common/FpsMeter.h"

#include "lost/gl/Buffer.h"
#include "lost/gl/ArrayBuffer.h"

using namespace std;
using namespace boost;
using namespace lost::common;
using namespace lost::event;
using namespace lost::math;
using namespace lost::gl;
using namespace lost;
using namespace lost::gl::utils;
using namespace lost::application;


FpsMeter fpsMeter;
shared_ptr<gl::ArrayBuffer<Vec2> > vb;

void init(shared_ptr<Event> event)
{
  DOUT("init");

  const int numVerts = 2;
  Vec2 verts[numVerts];
  verts[0] = Vec2(0,0);
  verts[1] = Vec2(150,200);
  GLsizeiptr sizeVerts = sizeof(verts);

  DOUT("sizeof(verts): "<<sizeVerts);

  vb.reset(new gl::ArrayBuffer<Vec2>);
  vb->bindBufferData(verts, numVerts);

  glEnableClientState(GL_VERTEX_ARRAY);GLDEBUG;
}

void redraw(shared_ptr<TimerEvent> event)
{
  // viewport / perspective
  int width = appInstance->displayAttributes.width;
  int height = appInstance->displayAttributes.height;
  glViewport(0, 0, width, height);GLDEBUG;
  set2DProjection(Vec2(0, 0), Vec2(width, height));GLDEBUG;

  // initial setup and clear
  glDisable(GL_DEPTH_TEST);GLDEBUG;
  glDisable(GL_TEXTURE_2D);GLDEBUG;
  glClearColor( 0.0, 0.0, 0.0, 0.0 );GLDEBUG;
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );GLDEBUG;

  // draw lines
  setColor(whiteColor);
  vb->bindVertexPointer();
  vb->draw(GL_LINES);

  // draw points
  glPointSize(5);
  setColor(redColor);
  vb->draw(GL_POINTS);
  
  fpsMeter.render(200,0,event->passedSec);
  
  glfwSwapBuffers();  
}

void key(shared_ptr<KeyEvent> event)
{
  if(event->key == K_ESCAPE)
    appInstance->quit();
}

int main(int argn, char** args)
{
  LogLevel( log_all );
  try
  {
    Application app;
    app.addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(key));
    app.addEventListener(ApplicationEvent::INIT(), init);
    Timer redrawTimer("redraw", 1.0/60.0);
    redrawTimer.addEventListener(TimerEvent::TIMER_FIRED(), receive<TimerEvent>(redraw));
    app.run();
  }
  catch (exception& e)
  {
    EOUT("exception: " << e.what());
  }

  return 0;
}
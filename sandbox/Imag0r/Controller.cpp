#include "Controller.h"
#include "lost/application/Application.h"
#include "lost/application/KeySym.h"


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

Controller::Controller(shared_ptr<Loader> inLoader) : loader(inLoader) {}

void Controller::redraw(shared_ptr<TimerEvent> event)
{
  glViewport(0, 0, appInstance->displayAttributes.width, appInstance->displayAttributes.height);GLDEBUG;
  set2DProjection(Vec2(0,0), Vec2(appInstance->displayAttributes.width, appInstance->displayAttributes.height));
  glClearColor(1,0,1,0);GLDEBUG;
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );GLDEBUG;
  
  glMatrixMode(GL_MODELVIEW);GLDEBUG;
  glLoadIdentity();GLDEBUG;
  glEnable(GL_TEXTURE_2D);GLDEBUG;
  glEnable(GL_BLEND);GLDEBUG;
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    GLDEBUG;
  
  glEnableClientState(GL_VERTEX_ARRAY);GLDEBUG;
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);GLDEBUG;
  drawRectTextured(Rect(10,10,texture->width,texture->height), *texture);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);GLDEBUG;
  
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);
  
  setColor(whiteColor);
  drawRectFilled(Rect(400,400,50,50));
  
  fpsMeter.render(2,2,event->passedSec);
  
  appInstance->swapBuffers();
}

void Controller::keyboard(shared_ptr<KeyEvent> inEvent)
{
  switch (inEvent->key)
  {
    case K_ESCAPE:appInstance->quit();break;
    default:break;
  }
}

void Controller::init(shared_ptr<ApplicationEvent> event)
{
  //setup resources
  //string filename = "gay_zombie.jpg";
  //string filename = "nomnomnom.jpg";
  //string filename = "buttonReleased.png";
  string filename = "stubs.jpg";
  bitmap = loader.load(filename);
  
  
  texture.reset(new Texture());
  texture->bind();
  texture->reset(0, GL_RGBA8, false, *bitmap);
  texture->wrap(GL_CLAMP_TO_EDGE);
  texture->filter(GL_LINEAR);
  
  DOUT("width: "<<texture->width<< " height: "<<texture->height);
  
  redrawTimer = new Timer("redrawTimer", 1.0/60.0);
  redrawTimer->addEventListener(TimerEvent::TIMER_FIRED(), receive<TimerEvent>(bind(&Controller::redraw, this, _1)));
}

#include "lost/common/Logger.h"
#include "lost/application/Application.h"
#include "lost/application/Timer.h"
#include "lost/application/TimerEvent.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/ApplicationEvent.h"
#include <boost/bind.hpp>
#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"
#include "lost/gl/Draw.h"
#include "lost/bitmap/BitmapLoader.h"
#include "lost/platform/Platform.h"
#include "lost/application/KeySym.h"
#include "lost/common/FpsMeter.h"

using namespace std;
using namespace lost::gl;
using namespace lost::math;
using namespace lost::common;
using namespace lost::event;
using namespace lost::application;
using namespace lost::platform;
using namespace lost::bitmap;
using namespace lost::resource;
using namespace lost::application;
using namespace boost;

struct Controller
{
BitmapLoader loader;
shared_ptr<Bitmap> bitmap;
shared_ptr<Texture> texture;
FpsMeter fpsMeter;

Controller(shared_ptr<Loader> inLoader) : loader(inLoader) {}

void redraw(shared_ptr<TimerEvent> event)
{
  glViewport(0, 0, appInstance->displayAttributes.width, appInstance->displayAttributes.height);GLDEBUG;
  lost::gl::utils::set2DProjection(lost::math::Vec2(0,0), lost::math::Vec2(appInstance->displayAttributes.width, appInstance->displayAttributes.height));
  glClearColor(1,0,1,0);GLDEBUG;
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );GLDEBUG;

  glMatrixMode(GL_MODELVIEW);GLDEBUG;
  glLoadIdentity();GLDEBUG;
  glEnable(GL_TEXTURE_2D);GLDEBUG;
  glEnable(GL_BLEND);GLDEBUG;
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    GLDEBUG;
  glColor3f(1, 1, 1);GLDEBUG;
  glBegin(GL_LINES);
  glVertex2f(0,0);
  glVertex2f(appInstance->displayAttributes.width, appInstance->displayAttributes.height);
  glEnd();
  
  drawRectTextured(Rect(10,10,texture->width,texture->height), *texture);

  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);
  fpsMeter.render(2,2,event->passedSec);

  glfwSwapBuffers();
}

void keyboard(shared_ptr<KeyEvent> inEvent)
{
    switch (inEvent->key)
    {
      case K_ESCAPE :
          appInstance->quit();
        break;
        default :
        break;
    }
}

void init(shared_ptr<ApplicationEvent> event)
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
    texture->wrap(GL_CLAMP);
    texture->filter(GL_LINEAR);
        
    
    DOUT("width: "<<texture->width<< " height: "<<texture->height);
}
};

int main(int argn, char** args)
{
  LogLevel( log_all );
  try
  {
    Application app;
    Controller controller(appInstance->loader);
    Timer redrawTimer("redrawTimer", 1.0/60.0);

    app.addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&Controller::keyboard, &controller, _1)));
    app.addEventListener(ApplicationEvent::INIT(), receive<ApplicationEvent>(bind(&Controller::init, &controller, _1)));
    redrawTimer.addEventListener(TimerEvent::TIMER_FIRED(), receive<TimerEvent>(bind(&Controller::redraw, &controller, _1)));
    
    app.run();
  }
  catch (exception& e)
  {
    EOUT("exception: " << e.what());
  }

  return 0;
}

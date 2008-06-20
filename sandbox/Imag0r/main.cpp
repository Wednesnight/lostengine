#include "lost/common/Logger.h"
#include "lost/application/Application.h"
#include "lost/application/Timer.h"
#include <boost/bind.hpp>
#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"
#include "lost/gl/Draw.h"
#include "lost/bitmap/BitmapLoader.h"
#include "lost/platform/Platform.h"
#include "lost/engine/KeySym.h"

using namespace std;
using namespace lost::gl;
using namespace lost::math;
using namespace lost::common;
using namespace lost::event;
using namespace lost::engine;
using namespace lost::platform;
using namespace lost::bitmap;
using namespace lost::application;
using namespace boost;

// global vars must be plain pointers, or the logger will crash because it'll 
// receive log messages during destructors when it's already shut down
// weird I know, we'll have to investigate
Application* app;
Bitmap* zombie1;
Texture* tex1;

void redraw(double delta, Timer* timer)
{
    glViewport(0, 0, app->displayAttributes.width, app->displayAttributes.height);GLDEBUG;
    lost::gl::utils::set2DProjection(lost::math::Vec2(0,0), lost::math::Vec2(app->displayAttributes.width, app->displayAttributes.height));
    glClearColor(1,1,1,0);GLDEBUG;
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );GLDEBUG;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    glVertex2f(0,0);
    glVertex2f(app->displayAttributes.width, app->displayAttributes.height);
    glEnd();

    drawRectTextured(Rect(10,10,tex1->width,tex1->height), *tex1);

    glfwSwapBuffers();
}

void keyboard(const KeyEvent& inEvent)
{
    switch (inEvent.key)
    {
      case lost::engine::K_ESCAPE :
        if(!inEvent.pressed)
        {
          app->quit();
        }
        break;
        default :
        break;
    }
}

int main(int argn, char** args)
{
  LogLevel( log_all );
  try
  {
    Application mainapp;
    BitmapLoader bitmapLoader;
    Timer t1("redrawTimer", 0.015);
    mainapp.key.connect(function<void(const KeyEvent& inEvent)>(keyboard));

    t1.action.connect(function<void(double, Timer*)>(redraw));
    app = &mainapp;
    
    //setup resources
    //string filename = "gay_zombie.jpg";
    //string filename = "nomnomnom.jpg";
    //string filename = "buttonReleased.png";
    string filename = "stubs.jpg";
    shared_ptr<Bitmap> z1 = bitmapLoader.loadFromPath(buildResourcePath(filename));
    
    zombie1 = z1.get();
    
    shared_ptr<Texture> ztex1(new Texture());
    ztex1->bind();
    ztex1->reset(0, GL_RGBA8, false, *z1);
    ztex1->wrap(GL_CLAMP);
    ztex1->filter(GL_LINEAR);
        
    tex1 = ztex1.get();
    
    DOUT("width: "<<tex1->width<< " height: "<<tex1->height);
    
    mainapp.run();
  }
  catch (exception& e)
  {
    EOUT("exception: " << e.what());
  }

  return 0;
}

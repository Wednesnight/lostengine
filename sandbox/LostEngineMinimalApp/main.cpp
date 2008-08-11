#include "lost/common/Logger.h"
#include "lost/application/Application.h"
#include "lost/application/Timer.h"
#include "lost/application/TimerEvent.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/application/TouchEvent.h"
#include <boost/bind.hpp>
#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"
#include "lost/gl/Draw.h"
#include "lost/bitmap/BitmapLoader.h"
#include "lost/platform/Platform.h"
#include "lost/application/KeySym.h"
#include "lost/common/FpsMeter.h"
#include "lost/model/parser/ParserOBJ.h"
#include "lost/model/Mesh.h"
#include "lost/model/MaterialOBJ.h"
#include "lost/model/RendererOBJ.h"

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
using namespace lost::model;
using namespace boost;


Timer* redrawTimer;


struct Controller
{
  BitmapLoader loader;
  shared_ptr<Bitmap> bitmap;
  shared_ptr<Texture> texture;
  FpsMeter fpsMeter;
  
  shared_ptr<parser::ParserOBJ> modelParser;
  shared_ptr<Mesh>              mesh;
  shared_ptr<MaterialOBJ>       material;
  shared_ptr<RendererOBJ>       modelRenderer;  
  
  Controller(shared_ptr<Loader> inLoader) : loader(inLoader) {}
  
  void redraw(shared_ptr<TimerEvent> event)
  {
    glViewport(0, 0, appInstance->displayAttributes.width, appInstance->displayAttributes.height);GLDEBUG;
    glClearColor(1,0,1,0);GLDEBUG;
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );GLDEBUG;
    glEnable(GL_DEPTH_TEST);GLDEBUG;
    glDisable(GL_TEXTURE_2D);GLDEBUG;

    lost::gl::utils::set3DProjection(appInstance->displayAttributes.width, appInstance->displayAttributes.height,
                                     Vec3(0.0f, 3.0f, 15.0f), Vec3(0.0f, 3.0f, 0.0f), Vec3(0,1,0),
                                     120, .1, 1000);
    glMatrixMode(GL_MODELVIEW);GLDEBUG;
    glLoadIdentity();GLDEBUG;
    
    GLfloat shininess[] = {128.0f};
    GLfloat ambient[]   = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat diffuse[]   = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat specular[]  = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat position[] = {1.0f, 1.0f, 5.0f, 0.0f};
//    GLfloat direction[] = {cameraDirection.x, cameraDirection.y, cameraDirection.z};
//    GLfloat cutoff[] = {180.0f};
    
    glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
//    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
//    glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, cutoff);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    
    //    lightingShader->enable();
    setColor(whiteColor);
    modelRenderer->render();
    //    lightingShader->disable();
    
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    
    setColor(whiteColor);
    drawAABB(mesh->box);
    glScalef(10.0f, 10.0f, 10.0f);
    
    drawAxes(Vec3(10.0f, 10.0f, 10.0f));

    lost::gl::utils::set2DProjection(lost::math::Vec2(0,0), lost::math::Vec2(appInstance->displayAttributes.width, appInstance->displayAttributes.height));
    glMatrixMode(GL_MODELVIEW);GLDEBUG;
    glLoadIdentity();GLDEBUG;
/*
    glEnable(GL_TEXTURE_2D);GLDEBUG;
    glEnable(GL_BLEND);GLDEBUG;
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    GLDEBUG;
    glColor4f(1, 1, 1,1);GLDEBUG;
//    drawLine(Vec2(0,0),Vec2(appInstance->displayAttributes.width, appInstance->displayAttributes.height));
    glEnableClientState(GL_VERTEX_ARRAY);GLDEBUG;
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);GLDEBUG;    
    drawLine(Vec2(0,0), Vec2(320,480));
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);GLDEBUG;
    drawRectTextured(Rect(10,10,texture->width,texture->height), *texture);
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    
    setColor(whiteColor);
    drawRectFilled(Rect(400,400,50,50));
*/    
    fpsMeter.render(2,2,event->passedSec);
    
    appInstance->swapBuffers();
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
    texture->reset(0, bitmap->format, false, *bitmap);
    texture->wrap(GL_CLAMP_TO_EDGE);
    texture->filter(GL_LINEAR);
    
    DOUT("width: "<<texture->width<< " height: "<<texture->height);
    
    std::string modelname = luabind::object_cast<std::string>(luabind::globals(*(appInstance->interpreter))["modelFilename"]);
    float       modelSize = luabind::object_cast<float>(luabind::globals(*(appInstance->interpreter))["modelSize"]);
    modelParser.reset(new parser::ParserOBJ(appInstance->loader));
    modelParser->parseMesh(modelname, mesh, material);
    modelRenderer.reset(new RendererOBJ(mesh, material));
    modelRenderer->size = modelSize;

    redrawTimer = new Timer("redrawTimer", 1.0/30.0);
    redrawTimer->addEventListener(TimerEvent::TIMER_FIRED(), receive<TimerEvent>(bind(&Controller::redraw, this, _1)));
  }
  
  void touches(shared_ptr<TouchEvent> event)
  {
    DOUT(event->type);
  }
};

int main(int argn, char** args)
{
  LogLevel( log_all );
  try
  {
    Application app;
    Controller controller(appInstance->loader);
    
    app.addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&Controller::keyboard, &controller, _1)));
    app.addEventListener(ApplicationEvent::INIT(), receive<ApplicationEvent>(bind(&Controller::init, &controller, _1)));
    app.addEventListener(TouchEvent::TOUCHES_BEGAN(), receive<TouchEvent>(bind(&Controller::touches, &controller, _1)));
    app.addEventListener(TouchEvent::TOUCHES_MOVED(), receive<TouchEvent>(bind(&Controller::touches, &controller, _1)));
    app.addEventListener(TouchEvent::TOUCHES_ENDED(), receive<TouchEvent>(bind(&Controller::touches, &controller, _1)));
    app.addEventListener(TouchEvent::TOUCHES_CANCELLED(), receive<TouchEvent>(bind(&Controller::touches, &controller, _1)));
    app.run();
  }
  catch (exception& e)
  {
    EOUT("exception: " << e.what());
  }
  
  return 0;
}

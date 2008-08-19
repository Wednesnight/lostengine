#include "lost/common/Logger.h"
#include "lost/application/Application.h"
#include "lost/application/Timer.h"
#include "lost/application/TimerEvent.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/application/TouchEvent.h"
#include "lost/application/AccelerometerEvent.h"
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
#include "lost/camera/Camera.h"

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
using namespace lost::camera;
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

  shared_ptr<Camera> camera;

  bool renderNormals;
  bool renderAABB;

  Controller(shared_ptr<Loader> inLoader)
  : loader(inLoader),
    renderNormals(false),
    renderAABB(false)
  {
  }
  
  void redraw(shared_ptr<TimerEvent> event)
  {
    glClearColor(0,0,0,0);GLDEBUG;
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );GLDEBUG;
    glEnable(GL_DEPTH_TEST);GLDEBUG;
    glDisable(GL_TEXTURE_2D);GLDEBUG;

    lost::gl::utils::set3DProjection(appInstance->displayAttributes.width, appInstance->displayAttributes.height,
                                     camera->position(), camera->target(), camera->up(),
                                     camera->fovY(), camera->depth().min, camera->depth().max);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    Vec4 vecAmbient  = luabind::object_cast<Vec4>(luabind::globals(*(appInstance->interpreter))["lightAmbient"]);
    Vec4 vecDiffuse  = luabind::object_cast<Vec4>(luabind::globals(*(appInstance->interpreter))["lightDiffuse"]);
    Vec4 vecSpecular = luabind::object_cast<Vec4>(luabind::globals(*(appInstance->interpreter))["lightSpecular"]);

    GLfloat shininess[]     = {luabind::object_cast<float>(luabind::globals(*(appInstance->interpreter))["lightShininess"])};
    GLfloat ambient[]       = {vecAmbient.x, vecAmbient.y, vecAmbient.z, vecAmbient.w};
    GLfloat diffuse[]       = {vecDiffuse.x, vecDiffuse.y, vecDiffuse.z, vecDiffuse.w};
    GLfloat specular[]      = {vecSpecular.x, vecSpecular.y, vecSpecular.z, vecSpecular.w};
    Vec3 lightPosition(0,3,15);
    GLfloat position[]      = {lightPosition.x, lightPosition.y, lightPosition.z, 0.0f};
    Vec3 lightDirection(0,0,-1);
    GLfloat direction[]     = {lightDirection.x, lightDirection.y, lightDirection.z};
    GLfloat cutoff[]        = {luabind::object_cast<float>(luabind::globals(*(appInstance->interpreter))["lightCutoff"])};
    
    glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
    glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, cutoff);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_RESCALE_NORMAL);
    
    setColor(whiteColor);
    modelRenderer->render();
    
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);

    if (renderNormals) modelRenderer->renderNormals();
    if (renderAABB) modelRenderer->renderAABB();
    
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

    camera.reset(new Camera());
    camera->position(Vec3(0,3,15));
    camera->target(Vec3(0,3,0));

    redrawTimer = new Timer("redrawTimer", 1.0/30.0);
    redrawTimer->addEventListener(TimerEvent::TIMER_FIRED(), receive<TimerEvent>(bind(&Controller::redraw, this, _1)));

    glViewport(0, 0, appInstance->displayAttributes.width, appInstance->displayAttributes.height);GLDEBUG;
  }
  
  void touches(shared_ptr<TouchEvent> event)
  {
    DOUT(event->type);
    if (event->touches.size() == 1)
    {
      static bool   initialized = false;
      static Vec2   lastPos(0,0);
      static double lastTap = 0.0;
      if (event->type == TouchEvent::TOUCHES_BEGAN())
      {
        initialized = true;
        lastPos     = event->touches[0]->location;

        if (lastTap > 0.0 && (event->touches[0]->timeStamp - lastTap) < 0.2) renderNormals = !renderNormals;
        lastTap = event->touches[0]->timeStamp;
      }
      else if (event->type == TouchEvent::TOUCHES_ENDED() || event->type == TouchEvent::TOUCHES_CANCELLED())
      {
        initialized = false;
      }
      else if (event->type == TouchEvent::TOUCHES_MOVED() && initialized)
      {
        float dx = (event->touches[0]->location.x - lastPos.x) * 0.1f;
        float dy = -1.0f * (event->touches[0]->location.y - lastPos.y) * 0.1f;
        
        camera->move(Vec3(dx, dy, 0.0f));
        lastPos = event->touches[0]->location;
      }
    }
    else if (event->touches.size() == 2)
    {
      static double lastTap = 0.0;
      if (event->type == TouchEvent::TOUCHES_BEGAN())
      {
        if (lastTap > 0.0 && (event->touches[0]->timeStamp - lastTap) < 0.2) renderAABB = !renderAABB;
        lastTap = event->touches[0]->timeStamp;
      }
    }
  }

  void accelerate(shared_ptr<AccelerometerEvent> event)
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
    app.addEventListener(AccelerometerEvent::DEVICE_ACCELERATED(), receive<AccelerometerEvent>(bind(&Controller::accelerate, &controller, _1)));
    app.run();
  }
  catch (exception& e)
  {
    EOUT("exception: " << e.what());
  }
  
  return 0;
}

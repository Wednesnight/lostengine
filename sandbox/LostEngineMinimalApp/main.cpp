#include <boost/bind.hpp>

#include "lost/common/Logger.h"
#include "lost/application/Application.h"
#include "lost/application/Timer.h"
#include "lost/application/TimerEvent.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/application/TouchEvent.h"
#include "lost/application/AccelerometerEvent.h"
#include "lost/application/ResizeEvent.h"
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

struct Controller
{
  Timer* redrawTimer;

  FpsMeter fpsMeter;
  
  shared_ptr<parser::ParserOBJ> modelParser;
  shared_ptr<Mesh>              mesh;
  shared_ptr<MaterialOBJ>       material;
  shared_ptr<RendererOBJ>       modelRenderer;  

  shared_ptr<Camera> camera;

  bool renderNormals;
  bool renderAABB;

  Vec4 vecAmbient;
  Vec4 vecDiffuse;
  Vec4 vecSpecular;
  GLfloat* shininess;
  GLfloat* ambient;
  GLfloat* diffuse;
  GLfloat* specular;
  Vec3 lightPosition;
  GLfloat* position;
  Vec3 lightDirection;
  GLfloat* direction;
  GLfloat* cutoff;
  
  
  Controller(shared_ptr<Loader> inLoader)
  : renderNormals(false),
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

    lightPosition = camera->position();
    position[0] = lightPosition.x;
    position[1] = lightPosition.y;
    position[2] = lightPosition.z;
    position[3] = 0.0f;
    lightDirection = camera->direction();
    direction[0] = lightDirection.x;
    direction[1] = lightDirection.y;
    direction[2] = lightDirection.z;

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
    fpsMeter.render(2,2,event->passedSec);

    appInstance->swapBuffers();
  }
  
  void init(shared_ptr<ApplicationEvent> event)
  {
/*
    std::string modelname = luabind::object_cast<std::string>(luabind::globals(*(appInstance->interpreter))["config"]["modelFilename"]);
    float       modelSize = luabind::object_cast<float>(luabind::globals(*(appInstance->interpreter))["config"]["modelSize"]);
    modelParser.reset(new parser::ParserOBJ(appInstance->loader));
    modelParser->parseMesh(modelname, mesh, material);
    modelRenderer.reset(new RendererOBJ(mesh, material));
    modelRenderer->size = modelSize;
*/
    mesh          = luabind::object_cast<shared_ptr<Mesh> >(luabind::globals(*(appInstance->interpreter))["config"]["modelMesh"]);
    material      = luabind::object_cast<shared_ptr<MaterialOBJ> >(luabind::globals(*(appInstance->interpreter))["config"]["modelMaterial"]);
    modelRenderer = luabind::object_cast<shared_ptr<RendererOBJ> >(luabind::globals(*(appInstance->interpreter))["config"]["modelRenderer"]);
    camera        = luabind::object_cast<shared_ptr<Camera> >(luabind::globals(*(appInstance->interpreter))["config"]["camera"]);

    vecAmbient  = luabind::object_cast<Vec4>(luabind::globals(*(appInstance->interpreter))["config"]["lightAmbient"]);
    vecDiffuse  = luabind::object_cast<Vec4>(luabind::globals(*(appInstance->interpreter))["config"]["lightDiffuse"]);
    vecSpecular = luabind::object_cast<Vec4>(luabind::globals(*(appInstance->interpreter))["config"]["lightSpecular"]);

    shininess     = new GLfloat[1];
    shininess[0] = luabind::object_cast<float>(luabind::globals(*(appInstance->interpreter))["config"]["lightShininess"]);
    ambient       = new GLfloat[4];
    ambient[0] = vecAmbient.x;
    ambient[1] = vecAmbient.y;
    ambient[2] = vecAmbient.z;
    ambient[3] = vecAmbient.w;
    diffuse  = new GLfloat[4];
    diffuse[0] = vecDiffuse.x;
    diffuse[1] = vecDiffuse.y;
    diffuse[2] = vecDiffuse.z;
    diffuse[3] = vecDiffuse.w;
    specular = new GLfloat[4];
    specular[0] = vecSpecular.x;
    specular[1] = vecSpecular.y;
    specular[2] = vecSpecular.z;
    specular[3] = vecSpecular.w;
    lightPosition = camera->position();
    position = new GLfloat[4];
    position[0] = lightPosition.x;
    position[1] = lightPosition.y;
    position[2] = lightPosition.z;
    position[3] = 0.0f;
    lightDirection = camera->direction();
    direction = new GLfloat[3];
    direction[0] = lightDirection.x;
    direction[1] = lightDirection.y;
    direction[2] = lightDirection.z;
    cutoff        = new GLfloat[1];
    cutoff[0] = luabind::object_cast<float>(luabind::globals(*(appInstance->interpreter))["config"]["lightCutoff"]);    
    
    glViewport (0, 0, appInstance->displayAttributes.width, appInstance->displayAttributes.height);

    redrawTimer = new Timer("redrawTimer", 1.0/30.0);
    redrawTimer->addEventListener(TimerEvent::TIMER_FIRED(), receive<TimerEvent>(bind(&Controller::redraw, this, _1)));

    appInstance->addEventListener(TouchEvent::TOUCHES_BEGAN(), receive<TouchEvent>(bind(&Controller::touches, this, _1)));
    appInstance->addEventListener(TouchEvent::TOUCHES_MOVED(), receive<TouchEvent>(bind(&Controller::touches, this, _1)));
    appInstance->addEventListener(TouchEvent::TOUCHES_ENDED(), receive<TouchEvent>(bind(&Controller::touches, this, _1)));
    appInstance->addEventListener(TouchEvent::TOUCHES_CANCELLED(), receive<TouchEvent>(bind(&Controller::touches, this, _1)));
  }
  
  void touches(shared_ptr<TouchEvent> event)
  {
    if (event->touches.size() == 1)
    {
      static double lastTap = 0.0;
      if (event->type == TouchEvent::TOUCHES_BEGAN())
      {
        if (lastTap > 0.0 && (event->touches[0]->timeStamp - lastTap) < 1.0)
        {
          renderNormals = !renderNormals;
          lastTap = 0.0;
        }
        else
        {
          lastTap = event->touches[0]->timeStamp;
        }
      }
    }
    else if (event->touches.size() == 2)
    {
      static bool   initialized = false;
      static double lastLength = 0.0;
      static double lastTap = 0.0;
      if (event->type == TouchEvent::TOUCHES_BEGAN())
      {
        initialized = true;
        lastLength  = len(event->touches[1]->location - event->touches[0]->location);
        
        if (lastTap > 0.0 && (event->touches[0]->timeStamp - lastTap) < 1.0)
        {
          renderAABB = !renderAABB;
          lastTap = 0.0;
        }
        else
        {
          lastTap = event->touches[0]->timeStamp;
        }
      }
      else if (event->type == TouchEvent::TOUCHES_MOVED() && initialized)
      {
        double currentLength = len(event->touches[1]->location - event->touches[0]->location);
        float delta = currentLength - lastLength;
        DOUT("moving " << delta << " units");
        lastLength = currentLength;
        camera->move(Vec3(0.0f, 0.0f, delta));
      }
      else if (event->type == TouchEvent::TOUCHES_ENDED() || event->type == TouchEvent::TOUCHES_CANCELLED())
      {
        initialized = false;
      }
    }
  }

};

int main(int argn, char** args)
{
  LogLevel( log_all );
  try
  {
    Application app;
    Controller controller(appInstance->loader);
    
    app.addEventListener(ApplicationEvent::INIT(), receive<ApplicationEvent>(bind(&Controller::init, &controller, _1)));
    app.run();
  }
  catch (exception& e)
  {
    EOUT("exception: " << e.what());
  }
  
  return 0;
}

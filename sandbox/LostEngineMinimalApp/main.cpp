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
#include "lost/bitmap/Bitmap.h"
#include "lost/platform/Platform.h"
#include "lost/application/KeySym.h"
#include "lost/common/FpsMeter.h"
#include "lost/model/obj/Renderer.h"
#include "lost/camera/Camera.h"
#include "lost/model/lsystem/Generator.h"
#include "lost/model/lsystem/Renderer.h"
#include "lost/event/Receive.h"

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
using namespace lost::model::obj;
using namespace lost::camera;
using namespace boost;

struct Controller
{
  lost::shared_ptr<lost::gl::State> renderState;

  Timer* redrawTimer;

  shared_ptr<FpsMeter> fpsMeter;
  
  shared_ptr<Renderer> modelRenderer;  
  shared_ptr<Camera>   camera;

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

  lost::lsystem::LSystem                            lsystem;
  lost::model::lsystem::Generator                   lsystemGenerator;
  lost::shared_ptr<lost::lsystem::LSystemState>    lsystemState;
  
  lost::shared_ptr<lost::model::Mesh>              dragonCurveMesh;
  lost::shared_ptr<lost::model::lsystem::Renderer> dragonCurve;
  
  lost::shared_ptr<lost::model::Mesh>              treeMesh;
  lost::shared_ptr<lost::model::lsystem::Renderer> tree;
  
  bool animate;
  
  Controller(shared_ptr<Loader> inLoader)
  : renderNormals(false),
    renderAABB(false),
    animate(true)
  {
    // generate dragon curve
    std::map<char, std::string> variableMap;
    variableMap['F'] = "FxyzFXYZFxyzFxyzF";
    lsystemState.reset(new lost::lsystem::LSystemState("FxyzFxyzFxyzF", variableMap, lost::math::Vec3(25,25,90)));
    lsystemState->reset();
    lsystem.advance(lsystemState, 5);
    dragonCurveMesh = lsystemGenerator.generate(lsystemState);
    // generate tree
    variableMap.clear();
    variableMap['f'] = "Fxyz[[f]XYZf]XYZF[XYZFf]xyzf";
    variableMap['F'] = "FF";
    lsystemState.reset(new lost::lsystem::LSystemState("f", variableMap, lost::math::Vec3(25,25,25)));
    lsystemState->reset();
    lsystem.advance(lsystemState, 5);
    treeMesh = lsystemGenerator.generate(lsystemState);
  }
  
  void redraw(shared_ptr<TimerEvent> event)
  {
    appInstance->context->pushState(renderState);
    appInstance->context->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    appInstance->context->set3DProjection(camera->position(), camera->target(), camera->up(),
                                          camera->fovY(), camera->depth());
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLineWidth(10.0f);
    appInstance->context->setColor(lost::common::Color(0,0,1,0.5));
    dragonCurve->size = 0.3f;
    static float dragonCurveAngle(0.0f);
    if (animate)
    {
      dragonCurveAngle += 0.25f;
      if (dragonCurveAngle >= 360) dragonCurveAngle -= 360.0f;
    }
    glTranslatef(0, 0, -20);
    glRotatef(-dragonCurveAngle, 0, 0, 1);
    dragonCurve->render();
    glLineWidth(1.0f);
    
    glLineWidth(3.0f);
    appInstance->context->setColor(lost::common::Color(1,0,0,0.5));
    tree->size = 0.1f;
    static float treeAngle(0.0f);
    if (animate)
    {
      treeAngle += 0.25f;
      if (treeAngle >= 360) treeAngle -= 360.0f;
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(treeAngle-45, 0, 0, 1);
    glRotatef(treeAngle, 0, 1, 0);
    tree->render();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(treeAngle+45, 0, 0, 1);
    glRotatef(treeAngle, 0, 1, 0);
    tree->render();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(treeAngle-135, 0, 0, 1);
    glRotatef(treeAngle, 0, 1, 0);
    tree->render();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(treeAngle+135, 0, 0, 1);
    glRotatef(treeAngle, 0, 1, 0);
    tree->render();
    glLineWidth(1.0f);
    
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
    
    appInstance->context->setColor(whiteColor);
    modelRenderer->render();
    
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);

    if (renderNormals) modelRenderer->renderNormals();
    if (renderAABB) modelRenderer->renderAABB();
    
    glScalef(10.0f, 10.0f, 10.0f);
    appInstance->context->drawAxes(Vec3(10.0f, 10.0f, 10.0f));

    appInstance->context->set2DProjection(lost::math::Vec2(0,0), lost::math::Vec2(appInstance->displayAttributes->width, appInstance->displayAttributes->height));
    glMatrixMode(GL_MODELVIEW);GLDEBUG;
    glLoadIdentity();GLDEBUG;
    fpsMeter->render(2,2,event->passedSec);

    appInstance->context->popState();

    appInstance->swapBuffers();
  }
  
  void preinit(shared_ptr<ApplicationEvent> event)
  {
    appInstance->addEventListener(ApplicationEvent::INIT(), receive<ApplicationEvent>(bind(&Controller::init, this, _1)));
  }
  
  void init(shared_ptr<ApplicationEvent> event)
  {
    DOUT("Controller::init()");

    renderState = appInstance->context->copyState();
    renderState->clearColor = blackColor;
    renderState->depthTest = true;
    renderState->texture2D = false;
    renderState->vertexArray = true;
    renderState->blend = true;
    renderState->blendSrc = GL_SRC_ALPHA;
    renderState->blendDest = GL_ONE_MINUS_SRC_ALPHA;
    
    modelRenderer = appInstance->config["modelRenderer"].as<shared_ptr<Renderer> >();
    camera        = appInstance->config["camera"].as<shared_ptr<Camera> >();

    vecAmbient  = appInstance->config["lightAmbient"].as<Vec4>(Vec4());
    vecDiffuse  = appInstance->config["lightDiffuse"].as<Vec4>(Vec4());
    vecSpecular = appInstance->config["lightSpecular"].as<Vec4>(Vec4());

    shininess     = new GLfloat[1];
    shininess[0] = appInstance->config["lightShininess"].as<float>(0.0f);
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
    cutoff       = new GLfloat[1];
    cutoff[0]    = appInstance->config["lightCutoff"].as<float>(0.0f);

    dragonCurve.reset(new lost::model::lsystem::Renderer(appInstance->context, dragonCurveMesh));
    tree.reset(new lost::model::lsystem::Renderer(appInstance->context, treeMesh));

    appInstance->addEventListener(ResizeEvent::MAIN_WINDOW_RESIZE(), receive<ResizeEvent>(bind(&Controller::resizeHandler, this, _1)));
    appInstance->addEventListener(TouchEvent::TOUCHES_BEGAN(), receive<TouchEvent>(bind(&Controller::touches, this, _1)));
    appInstance->addEventListener(TouchEvent::TOUCHES_MOVED(), receive<TouchEvent>(bind(&Controller::touches, this, _1)));
    appInstance->addEventListener(TouchEvent::TOUCHES_ENDED(), receive<TouchEvent>(bind(&Controller::touches, this, _1)));
    appInstance->addEventListener(TouchEvent::TOUCHES_CANCELLED(), receive<TouchEvent>(bind(&Controller::touches, this, _1)));

    fpsMeter.reset(new FpsMeter(appInstance->context)),

    redrawTimer = new Timer("redrawTimer", 1.0/30.0);
    redrawTimer->addEventListener(TimerEvent::TIMER_FIRED(), receive<TimerEvent>(bind(&Controller::redraw, this, _1)));
  }
  
  void touches(shared_ptr<TouchEvent> event)
  {
    static bool moved = false;
    if (event->touches.size() == 1)
    {
      if (event->type == TouchEvent::TOUCHES_BEGAN())
      {
        moved = false;
      }
      else if (event->type == TouchEvent::TOUCHES_MOVED())
      {
        moved = true;
      }
      else if (!moved && event->type == TouchEvent::TOUCHES_ENDED())
      {
        animate = !animate;
      }
    }
  }

  void resizeHandler(shared_ptr<ResizeEvent> event)
  {
    glViewport (0, 0, event->width, event->height); 
  }

};

int main(int argn, char** args)
{
  LogLevel( log_all );
  try
  {
    Application app;
    Controller controller(appInstance->loader);
    
    app.addEventListener(ApplicationEvent::PREINIT(), receive<ApplicationEvent>(bind(&Controller::preinit, &controller, _1)));
    app.run();
  }
  catch (std::exception& e)
  {
    EOUT("exception: " << e.what());
  }
  
  return 0;
}

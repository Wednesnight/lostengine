#ifndef OBJECT0R_H
#define OBJECT0R_H

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/signal.hpp>

#include "lost/common/Logger.h"
#include "lost/application/Application.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/application/TimerEvent.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/MouseEvent.h"
#include "lost/application/KeySym.h"
#include "lost/application/ResizeEvent.h"
#include "lost/application/TimerEvent.h"
#include "lost/gl/Utils.h"
#include "lost/gl/Draw.h"
#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"
#include "lost/math/Vec4.h"
#include "lost/math/Matrix.h"
#include "lost/event/EventDispatcher.h"
#include "lost/camera/Camera.h"
#include "lost/common/FpsMeter.h"
#include "lost/resource/File.h"
#include "lost/model/obj/Renderer.h"
#include "lost/gl/Buffer.h"
#include "lost/gl/ArrayBuffer.h"
#include "lost/gl/Draw.h"
#include "lost/gl/ElementArrayBuffer.h"
#include "lost/platform/Platform.h"
#include "lost/application/Timer.h"
#include "lost/lua/lua.h"
#include "lost/model/lsystem/Generator.h"
#include "lost/model/lsystem/Renderer.h"
#include "lost/gl/Context.h"

using namespace boost;
using namespace lost::application;
using namespace lost::common;
using namespace lost::event;
using namespace lost::gl;
using namespace lost::gl::utils;
using namespace lost::math;
using namespace lost::model::obj;
using namespace lost::camera;
using namespace std;

struct Object0r
{
  Timer                     renderTimer;
  boost::signal<void(void)> quit;
  shared_ptr<Camera>        camera;
  FpsMeter                  fpsMeter;
  
  shared_ptr<Renderer>      modelRenderer;

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
  boost::shared_ptr<lost::model::lsystem::Renderer> lsystemRenderer;
  boost::shared_ptr<lost::lsystem::LSystemState>    lsystemState;
  boost::shared_ptr<lost::model::Mesh>              lsystemMesh;
  
  Object0r(Application& app)
  : renderTimer("render", 0.015),
    renderNormals(false),
    renderAABB(false)
  {
    app.addEventListener(ApplicationEvent::PREINIT(), receive<Event>(bind(&Object0r::preinit, this, _1)));

    std::map<char, std::string> variableMap;
    variableMap['F'] = "FzXFZxFzXFzXF";
    lsystemState.reset(new lost::lsystem::LSystemState("FzFzFzF", variableMap, lost::math::Vec3(5,0,90)));
    lsystemState->reset();
    lsystem.advance(lsystemState, 6);
    lsystemMesh = lsystemGenerator.generate(lsystemState);
  }
  
  GLenum getModelDisplay(std::string& which)
  {
    if (which == "points")
    {
      return GL_POINT;
    }
    else if (which == "wireframe")
    {
      return GL_LINE;
    }
    else
    {
      return GL_FILL;
    }
    
  }
  
  void preinit(shared_ptr<Event> event)
  {
    appInstance->addEventListener(ApplicationEvent::INIT(), receive<Event>(bind(&Object0r::init, this, _1)));
  }
  
  void init(shared_ptr<Event> event)
  {
    glfwSetMousePos(appInstance->displayAttributes.width/2, appInstance->displayAttributes.height/2);
    glfwDisable(GLFW_MOUSE_CURSOR);

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
    
    lsystemRenderer.reset(new lost::model::lsystem::Renderer(appInstance->context, lsystemMesh));

    appInstance->addEventListener(KeyEvent::KEY_UP(), receive<KeyEvent>(bind(&Object0r::keyHandler, this, _1)));
    appInstance->addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&Object0r::keyHandler, this, _1)));
    appInstance->addEventListener(ResizeEvent::MAIN_WINDOW_RESIZE(), receive<ResizeEvent>(bind(&Object0r::resizeHandler, this, _1)));
    quit.connect(bind(&Application::quit, appInstance));
    
    renderTimer.addEventListener(TimerEvent::TIMER_FIRED(), receive<TimerEvent>(bind(&Object0r::render, this, _1)));
  }
  
  void render(shared_ptr<TimerEvent> event)
  {
    boost::shared_ptr<lost::gl::State> newState = appInstance->context->copyState();
    newState->depthTest = true;
    newState->texture2D = false;
    newState->vertexArray = true;
    appInstance->context->pushState(newState);

    glClearColor(0.0, 0.0, 0.0, 0.0);GLDEBUG;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);GLDEBUG;

    set3DProjection(appInstance->displayAttributes.width, appInstance->displayAttributes.height,
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
    lsystemRenderer->render();

    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);

    if (renderNormals) modelRenderer->renderNormals();
    if (renderAABB) modelRenderer->renderAABB();

    glScalef(10.0f, 10.0f, 10.0f);
    drawAxes(Vec3(10.0f, 10.0f, 10.0f));
    
    set2DProjection(Vec2(0,0), Vec2(appInstance->displayAttributes.width, appInstance->displayAttributes.height));
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    fpsMeter.render(appInstance->displayAttributes.width - fpsMeter.width, 0, event->passedSec);
    
    appInstance->context->popState();

    appInstance->swapBuffers();
  }
  
  void keyHandler(shared_ptr<KeyEvent> event)
  {
    if (event->pressed)
    {
      switch (event->key)
      {
        case K_ESCAPE:
          quit();
          break;
        case K_F1:
          modelRenderer->renderModeFront = GL_POINT;
          modelRenderer->renderModeBack  = GL_POINT;
          break;
        case K_F2:
          modelRenderer->renderModeFront = GL_LINE;
          modelRenderer->renderModeBack  = GL_LINE;
          break;
        case K_F3:
          modelRenderer->renderModeFront = GL_FILL;
          modelRenderer->renderModeBack  = GL_FILL;
          break;
        case K_F5:
          {
            std::stringstream file;
            file << "Desktop/Object0r_" << (unsigned int)lost::platform::currentTimeMilliSeconds() << ".tga";
            lost::gl::utils::saveScreenshotTGA(appInstance->displayAttributes, lost::platform::buildUserDataPath(file.str()));
          }
          break;
        case K_N:
          renderNormals = !renderNormals;
          break;
        case K_B:
          renderAABB = !renderAABB;
          break;
      }
    }
  }

  void resizeHandler(shared_ptr<ResizeEvent> event)
  {
    glViewport (0, 0, event->width, event->height); 
  }

};

#endif

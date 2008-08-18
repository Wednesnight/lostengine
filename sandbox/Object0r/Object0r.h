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
#include "lost/gl/ShaderProgram.h"
#include "lost/gl/ShaderHelper.h"
#include "lost/camera/Camera.h"
#include "lost/common/FpsMeter.h"
#include "lost/resource/File.h"
#include "lost/model/parser/ParserOBJ.h"
#include "lost/model/Mesh.h"
#include "lost/model/MaterialOBJ.h"
#include "lost/model/RendererOBJ.h"
#include "lost/gl/Buffer.h"
#include "lost/gl/ArrayBuffer.h"
#include "lost/gl/Draw.h"
#include "lost/gl/ElementArrayBuffer.h"
#include "lost/platform/Platform.h"
#include "lost/application/Timer.h"
#include "lost/lua/lua.h"

using namespace boost;
using namespace lost::application;
using namespace lost::common;
using namespace lost::event;
using namespace lost::gl;
using namespace lost::gl::utils;
using namespace lost::math;
using namespace lost::model;
using namespace lost::camera;
using namespace std;

struct Object0r
{
  Timer                     renderTimer;
  boost::signal<void(void)> quit;
  shared_ptr<Camera>        camera;
  FpsMeter                  fpsMeter;
  
  shared_ptr<ShaderProgram> lightingShader;

  shared_ptr<parser::ParserOBJ> modelParser;
  shared_ptr<Mesh>              mesh;
  shared_ptr<MaterialOBJ>       material;
  shared_ptr<RendererOBJ>       modelRenderer;

  bool renderNormals;
  bool renderAABB;
  
  Object0r(Application& app)
  : renderTimer("render", 0.015),
    renderNormals(false),
    renderAABB(false)
  {
    app.addEventListener(ApplicationEvent::INIT(), receive<Event>(bind(&Object0r::init, this, _1)));
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
  
  void init(shared_ptr<Event> event)
  {
    glfwSetMousePos(appInstance->displayAttributes.width/2, appInstance->displayAttributes.height/2);
    glfwDisable(GLFW_MOUSE_CURSOR);

    camera.reset(new Camera());
    camera->position(Vec3(0,3,15));
    camera->target(Vec3(0,3,0));

    std::string modelname         = luabind::object_cast<std::string>(luabind::globals(*(appInstance->interpreter))["modelFilename"]);
    float       modelSize         = luabind::object_cast<float>(luabind::globals(*(appInstance->interpreter))["modelSize"]);
    std::string modelDisplayFront = luabind::object_cast<std::string>(luabind::globals(*(appInstance->interpreter))["modelDisplayFront"]);
    std::string modelDisplayBack  = luabind::object_cast<std::string>(luabind::globals(*(appInstance->interpreter))["modelDisplayBack"]);

    modelParser.reset(new parser::ParserOBJ(appInstance->loader));
    modelParser->parseMesh(modelname, mesh, material);
    modelRenderer.reset(new RendererOBJ(mesh, material));
    modelRenderer->size            = modelSize;
    modelRenderer->renderModeFront = getModelDisplay(modelDisplayFront);
    modelRenderer->renderModeBack  = getModelDisplay(modelDisplayBack);

    shaderInit();

    appInstance->addEventListener(KeyEvent::KEY_UP(), receive<KeyEvent>(bind(&Object0r::keyHandler, this, _1)));
    appInstance->addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&Object0r::keyHandler, this, _1)));
    appInstance->addEventListener(MouseEvent::MOUSE_UP(), receive<MouseEvent>(bind(&Object0r::mouseClickHandler, this, _1)));
    appInstance->addEventListener(MouseEvent::MOUSE_DOWN(), receive<MouseEvent>(bind(&Object0r::mouseClickHandler, this, _1)));
    appInstance->addEventListener(MouseEvent::MOUSE_MOVE(), receive<MouseEvent>(bind(&Object0r::mouseMoveHandler, this, _1)));
    appInstance->addEventListener(ResizeEvent::MAIN_WINDOW_RESIZE(), receive<ResizeEvent>(bind(&Object0r::resizeHandler, this, _1)));
    quit.connect(bind(&Application::quit, appInstance));
    
    renderTimer.addEventListener(TimerEvent::TIMER_FIRED(), receive<TimerEvent>(bind(&Object0r::render, this, _1)));
  }
  
  void shaderInit()
  {
    lightingShader = loadShader(appInstance->loader, "lighting");
    lightingShader->enable();
    lightingShader->validate();
    if(!lightingShader->validated())
    {
      DOUT("Problem found during validation: \n"<<lightingShader->log())
    }
    else
    {
      DOUT("Program validated OK");
    }
    (*lightingShader)["LightPosition"] = Vec3(0,5,-5);
    (*lightingShader)["LightColor"]    = Color(1,1,1);
    (*lightingShader)["EyePosition"]   = Vec3(camera->position().x, camera->position().y, camera->position().z);
    (*lightingShader)["Specular"]      = Color(1,1,1);
    (*lightingShader)["Ambient"]       = Color(1,.1,.1);
    (*lightingShader)["SurfaceColor"]  = Color(1,1,.1);
    (*lightingShader)["Kd"]            = 0.8f;
    lightingShader->disable();
  }
  
  void render(shared_ptr<TimerEvent> event)
  {
    glClearColor(0.0, 0.0, 0.0, 0.0);GLDEBUG;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);GLDEBUG;
    glEnable(GL_DEPTH_TEST);GLDEBUG;
    glDisable(GL_TEXTURE_2D);GLDEBUG;

    static double lastSec = 0;
    if (lastSec >= 1)
    {
      DOUT("camera->position(): " << camera->position());
      DOUT("camera->target()  : " << camera->target());
      DOUT("camera->up()      : " << camera->up());
      DOUT("camera->fovY()    : " << camera->fovY());
      DOUT("camera->depth()   : " << camera->depth());
      DOUT("camera->rotation(): " << camera->rotation());
      lastSec = 0;
    }
    else
    {
      lastSec += event->passedSec;
    }

    set3DProjection(appInstance->displayAttributes.width, appInstance->displayAttributes.height,
                    camera->position(), camera->target(), camera->up(),
                    camera->fovY(), camera->depth().min, camera->depth().max);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    Vec4 vecAmbient  = luabind::object_cast<Vec4>(luabind::globals(*(appInstance->interpreter))["lightAmbient"]);
    Vec4 vecDiffuse  = luabind::object_cast<Vec4>(luabind::globals(*(appInstance->interpreter))["lightDiffuse"]);
    Vec4 vecSpecular = luabind::object_cast<Vec4>(luabind::globals(*(appInstance->interpreter))["lightSpecular"]);

    GLfloat shininess[]  = {luabind::object_cast<float>(luabind::globals(*(appInstance->interpreter))["lightShininess"])};
    GLfloat ambient[]    = {vecAmbient.x, vecAmbient.y, vecAmbient.z, vecAmbient.w};
    GLfloat diffuse[]    = {vecDiffuse.x, vecDiffuse.y, vecDiffuse.z, vecDiffuse.w};
    GLfloat specular[]   = {vecSpecular.x, vecSpecular.y, vecSpecular.z, vecSpecular.w};
    Vec3 cameraPosition  = camera->position();
    GLfloat position[]   = {cameraPosition.x, cameraPosition.y, cameraPosition.z, 0.0f};
    Vec3 cameraDirection = camera->direction();
    GLfloat direction[]  = {cameraDirection.x, cameraDirection.y, cameraDirection.z};
    GLfloat cutoff[]     = {luabind::object_cast<float>(luabind::globals(*(appInstance->interpreter))["lightCutoff"])};
    
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
    
//    lightingShader->enable();
    setColor(whiteColor);
    modelRenderer->render();
//    lightingShader->disable();

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
    
    appInstance->swapBuffers();
  }
  
  void keyHandler(shared_ptr<KeyEvent> event)
  {
    DOUT("key: " << event->key);
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
        case K_W:
          camera->move(Vec3(0,0,-0.25));
          break;
        case K_A:
          camera->move(Vec3(-0.25,0,0));
          break;
        case K_S:
          camera->move(Vec3(0,0,0.25));
          break;
        case K_D:
          camera->move(Vec3(0.25,0,0));
          break;
        case K_Q:
          camera->move(Vec3(0,-0.25,0));
          break;
        case K_E:
          camera->move(Vec3(0,0.25,0));
          break;
      }
    }
  }

  void mouseClickHandler(shared_ptr<MouseEvent> event)
  {
  }
  
  void mouseMoveHandler(shared_ptr<MouseEvent> event)
  {
    static bool moveInitialized = false;
    static Vec2 mousePos(0,0);
    
    if (!moveInitialized)
    {
      mousePos        = event->pos;
      moveInitialized = true;
    }
    else
    {
      DOUT("event->pos: " << event->pos);
      DOUT("mousePos  : " << mousePos);
      // x-axis rotation
      float dx = -1.0f * (event->pos.y - mousePos.y) * 0.1f;
      // y-axis rotation
      float dy = (event->pos.x - mousePos.x) * 0.1f;
      
      camera->rotate(Vec3(dx, dy, 0.0f));
      mousePos = event->pos;
    }
  }
  
  void resizeHandler(shared_ptr<ResizeEvent> event)
  {
    glViewport (0, 0, event->width, event->height); 
  }

};

#endif


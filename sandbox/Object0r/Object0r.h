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
using namespace std;

namespace lost
{
  namespace gl
  {
    struct CameraController
    {
      Camera camera;
      
      float              fovX;
      DisplayAttributes& display;
      Vec2               depth;
      Vec3               acceleration;
      Vec3               velocity;
      
      Vec2 mousePos;
      Vector3 direction;
      
      Timer cameraTimer;
      
      
      CameraController(DisplayAttributes& inDisplay, EventDispatcher& inDispatcher)
      : fovX(60.0f),
        display(inDisplay),
        depth(0.1f, 100.0f),
        acceleration(4.0f, 4.0f, 4.0f),
        velocity(1.0f, 1.0f, 1.0f),
        mousePos(0,0),
        direction(0.0f, 0.0f, 0.0f),
        cameraTimer("CameraController::cameraTimer", 0.025f)
      {
        camera.setBehavior(Camera::CAMERA_BEHAVIOR_SPECTATOR);
        camera.perspective(fovX, display.width / display.height, depth.min, depth.max);
        camera.setPosition(Vector3(0.0f, 0.0f, 10.0f));
        camera.setAcceleration(Vector3(acceleration.x, acceleration.y, acceleration.z));
        camera.setVelocity(Vector3(velocity.x, velocity.y, velocity.z));

        inDispatcher.addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(boost::bind(&CameraController::keyPressed, this, _1)));
        inDispatcher.addEventListener(KeyEvent::KEY_UP(), receive<KeyEvent>(boost::bind(&CameraController::keyPressed, this, _1)));
        inDispatcher.addEventListener(MouseEvent::MOUSE_MOVE(), receive<MouseEvent>(boost::bind(&CameraController::mouseMoved, this, _1)));
        
        cameraTimer.addEventListener(TimerEvent::TIMER_FIRED(), receive<TimerEvent>(boost::bind(&CameraController::updateCamera, this, _1)));
      }
      
      void updateCamera(boost::shared_ptr<TimerEvent> event)
      {
        camera.updatePosition(direction, event->passedSec);
      }
      
      void mouseMoved(boost::shared_ptr<MouseEvent> event)
      {
        static bool moveInitialized = false;
        
        if (!moveInitialized)
        {
          moveInitialized = true;
          mousePos = event->pos;
        }
        else
        {
          float dx = -(event->pos.x - mousePos.x);
          float dy = event->pos.y - mousePos.y;
          
          camera.rotateSmoothly(dx, dy, 0.0f);
          mousePos = event->pos;
        }
      }
      
      void keyPressed(boost::shared_ptr<KeyEvent> event)
      {
        Vector3 velocity = camera.getCurrentVelocity();
        direction = Vector3(0.0f, 0.0f, 0.0f);
        
        if (event->pressed)
        {
          switch (event->key)
          {
            case K_NUMPAD_PLUS:
              camera.setRotationSpeed(camera.getRotationSpeed() + 0.01f);
              if (camera.getRotationSpeed() > 1.0f)
                camera.setRotationSpeed(1.0f);
              break;
              
            case K_NUMPAD_MINUS:            
              camera.setRotationSpeed(camera.getRotationSpeed() - 0.01f);
              if (camera.getRotationSpeed() <= 0.0f)
                camera.setRotationSpeed(0.01f);
              break;
              
            case K_BACKSPACE:
              if (event->pressed) camera.undoRoll();
              break;
              
            case K_W:
              if (!event->repeat) camera.setCurrentVelocity(velocity.x, velocity.y, 0.0f);
              direction.z += 1.0f;
              break;
              
            case K_S:
              if (!event->repeat) camera.setCurrentVelocity(velocity.x, velocity.y, 0.0f);
              direction.z -= 1.0f;
              break;
              
            case K_A:
              if (!event->repeat) camera.setCurrentVelocity(0.0f, velocity.y, velocity.z);
              direction.x -= 1.0f;
              break;
              
            case K_D:
              if (!event->repeat) camera.setCurrentVelocity(0.0f, velocity.y, velocity.z);
              direction.x += 1.0f;
              break;
              
            case K_Q:
              if (!event->repeat) camera.setCurrentVelocity(velocity.x, 0.0f, velocity.z);
              direction.y -= 1.0f;
              break;
              
            case K_E:
              if (!event->repeat) camera.setCurrentVelocity(velocity.x, 0.0f, velocity.z);
              direction.y += 1.0f;
              break;
          }
        }
      }
    };
    
  }
}

struct Object0r
{
  boost::signal<void(void)>    quit;
  shared_ptr<CameraController> camera;
  FpsMeter                     fpsMeter;
  
  shared_ptr<ShaderProgram> lightingShader;

  shared_ptr<parser::ParserOBJ> modelParser;
  shared_ptr<Mesh>              mesh;
  shared_ptr<MaterialOBJ>       material;
  shared_ptr<RendererOBJ>       modelRenderer;

  Object0r() {}
  
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
    camera.reset(new CameraController(appInstance->displayAttributes, *appInstance));

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
    (*lightingShader)["EyePosition"]   = Vec3(camera->camera.getPosition().x, camera->camera.getPosition().y, camera->camera.getPosition().z);
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
    
    glEnableClientState(GL_VERTEX_ARRAY);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMultMatrixf(&camera->camera.getProjectionMatrix()[0][0]);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMultMatrixf(&camera->camera.getViewMatrix()[0][0]);

    GLfloat ambient[]   = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat diffuse[]   = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat specular[]  = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat shininess[] = {100.0f};
    GLfloat position[]  = {1.0f, 1.0f, 1.0f, 0.0f};
    glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

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
      }
    }
  }
  
  void resizeHandler(shared_ptr<ResizeEvent> event)
  {
    glViewport (0, 0, event->width, event->height); 
  }

};

#endif


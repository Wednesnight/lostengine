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
#include "lost/math/Matrix.h"
#include "lost/math/Quaternion.h"
#include "lost/event/EventDispatcher.h"
#include "lost/gl/ShaderProgram.h"
#include "lost/gl/ShaderHelper.h"
#include "lost/camera/Camera.h"
#include "lost/common/FpsMeter.h"
#include "lost/resource/File.h"
#include "lost/model/Mesh.h"
#include "lost/model/Vertex.h"
#include "lost/model/Parser.h"
#include "lost/gl/Buffer.h"
#include "lost/gl/ArrayBuffer.h"

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
      Vec2               zoom;
      Vec3               acceleration;
      Vec3               velocity;
      
      Vec2 mousePos;
      Vector3 direction;
      
      Timer cameraTimer;
      
      
      CameraController(DisplayAttributes& inDisplay, EventDispatcher& inDispatcher)
      : fovX(60.0f),
        display(inDisplay),
        depth(0.1f, 100.0f),
        zoom(1.5f, 5.0f),
        acceleration(4.0f, 4.0f, 4.0f),
        velocity(1.0f, 1.0f, 1.0f),
        mousePos(0,0),
        direction(0.0f, 0.0f, 0.0f),
        cameraTimer("CameraController::cameraTimer", 0.025f)
      {
        camera.setBehavior(Camera::CAMERA_BEHAVIOR_SPECTATOR);
        
        camera.perspective(fovX, display.width / display.height, depth.x, depth.y);
        
        camera.setPosition(Vector3(0.0f, 0.0f, 5.0f));
        camera.setOrbitMinZoom(zoom.x);
        camera.setOrbitMaxZoom(zoom.y);
        camera.setOrbitOffsetDistance(zoom.x + (zoom.y - zoom.x) * 0.3f);
        
        camera.setAcceleration(Vector3(acceleration.x, acceleration.y, acceleration.z));
        camera.setVelocity(Vector3(velocity.x, velocity.y, velocity.z));
        
        //        g_cameraBoundsMax.set(FLOOR_WIDTH / 2.0f, 4.0f, FLOOR_HEIGHT / 2.0f);
        //        g_cameraBoundsMin.set(-FLOOR_WIDTH / 2.0f, cameraOffset, -FLOOR_HEIGHT / 2.0f);
        
        inDispatcher.addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(boost::bind(&CameraController::keyPressed, this, _1)));
        inDispatcher.addEventListener(KeyEvent::KEY_UP(), receive<KeyEvent>(boost::bind(&CameraController::keyPressed, this, _1)));
        inDispatcher.addEventListener(MouseEvent::MOUSE_MOVE(), receive<MouseEvent>(boost::bind(&CameraController::mouseMoved, this, _1)));
        
        cameraTimer.addEventListener(TimerEvent::TIMER_FIRED(), receive<TimerEvent>(boost::bind(&CameraController::updateCamera, this, _1)));
      }
      
      void updateCamera(boost::shared_ptr<TimerEvent> event)
      {
        camera.updatePosition(direction, event->passedSec);
        //        DOUT("camera.position: " << camera.getPosition().x << ", " << camera.getPosition().y << ", " << camera.getPosition().z);
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
              camera.setCurrentVelocity(velocity.x, velocity.y, 0.0f);
              direction.z += 1.0f;
              break;
              
            case K_S:
              camera.setCurrentVelocity(velocity.x, velocity.y, 0.0f);
              direction.z -= 1.0f;
              break;
              
            case K_A:
              camera.setCurrentVelocity(0.0f, velocity.y, velocity.z);
              direction.x -= 1.0f;
              break;
              
            case K_D:
              camera.setCurrentVelocity(0.0f, velocity.y, velocity.z);
              direction.x += 1.0f;
              break;
              
            case K_Q:
              camera.setCurrentVelocity(velocity.x, 0.0f, velocity.z);
              direction.y -= 1.0f;
              break;
              
            case K_E:
              camera.setCurrentVelocity(velocity.x, 0.0f, velocity.z);
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
  shared_ptr<Mesh>             mesh;
  
  shared_ptr<ShaderProgram>        lightingShader;
  shared_ptr<ArrayBuffer<Vertex> > vertexBuffer;
  
  void init(shared_ptr<Event> event)
  {
    glfwSetMousePos(appInstance->displayAttributes.width/2, appInstance->displayAttributes.height/2);
    glfwDisable(GLFW_MOUSE_CURSOR);
    camera.reset(new CameraController(appInstance->displayAttributes, *appInstance));

    mesh = parser::parse(appInstance->loader->load("cube.obj")->str());
    DOUT(*mesh);

    shaderInit();
    bufferInit();
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
  
  void bufferInit()
  {
    vertexBuffer.reset(new ArrayBuffer<Vertex>);
    vertexBuffer->bindBufferData(mesh->vertices, mesh->vertexCount);
    
    glEnableClientState(GL_VERTEX_ARRAY);GLDEBUG;
  }
  
  void render(shared_ptr<TimerEvent> event)
  {
    glClearColor( 0.0, 0.0, 0.0, 0.0 );GLDEBUG;
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );GLDEBUG;
    glEnable(GL_DEPTH_TEST);GLDEBUG;
    glDisable(GL_TEXTURE_2D);GLDEBUG;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMultMatrixf(&camera->camera.getProjectionMatrix()[0][0]);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMultMatrixf(&camera->camera.getViewMatrix()[0][0]);
    
    lightingShader->enable();
    vertexBuffer->bindVertexPointer();
    // draw mesh vertices as points
    glPointSize(5);
    setColor(whiteColor);
    vertexBuffer->draw(GL_POINTS);
    lightingShader->disable();

    /* no cubes anymore
     glutSolidCube(1.0);
     glTranslatef(2.0f, 0.0f, 0.0f);
     glutSolidCube(1.0);
     glTranslatef(-4.0f, 0.0f, 0.0f);
     glutSolidCube(1.0);
     glTranslatef(2.0f, 2.0f, 0.0f);
     glutSolidCube(1.0);
     glTranslatef(0.0f, -4.0f, 0.0f);
     glutSolidCube(1.0);
     glTranslatef(0.0f, 2.0f, 2.0f);
     glutSolidCube(1.0);
     glTranslatef(0.0f, 0.0f, -4.0f);
     glutSolidCube(1.0);
     glTranslatef(0.0f, 0.0f, 2.0f);
     */
    
    // Draw the positive side of the lines x,y,z
    glBegin(GL_LINES);
    glColor3f(0.0f, 1.0f, 0.0f);                // Green for x axis
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(10.0f, 0.0f, 0.0f);
    glColor3f(1.0f, 0.0f, 0.0f);                // Red for y axis
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 10.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);                // Blue for z axis
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 10.0f);
    glEnd();
    
    // Dotted lines for the negative sides of x,y,z coordinates
    glEnable(GL_LINE_STIPPLE);                // Enable line stipple to use a dotted pattern for the lines
    glLineStipple(1, 0x0101);                    // Dotted stipple pattern for the lines
    glBegin(GL_LINES);
    glColor3f(0.0f, 1.0f, 0.0f);                    // Green for x axis
    glVertex3f(-10.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glColor3f(1.0f, 0.0f, 0.0f);                    // Red for y axis
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, -10.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);                    // Blue for z axis
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, -10.0f);
    glEnd();
    glDisable(GL_LINE_STIPPLE);                // Disable the line stipple
    
    set2DProjection(Vec2(0,0), Vec2(appInstance->displayAttributes.width, appInstance->displayAttributes.height));
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    fpsMeter.render(appInstance->displayAttributes.width - fpsMeter.width, 0, event->passedSec);
    
    glfwSwapBuffers();  
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
      }
    }
  }
  
  void resizeHandler(shared_ptr<ResizeEvent> event)
  {
    glViewport (0, 0, event->width, event->height); 
  }

};

#endif


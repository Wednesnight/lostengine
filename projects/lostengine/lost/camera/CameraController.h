#ifndef LOST_CAMERA_CAMERACONTROLLER_H
#define LOST_CAMERA_CAMERACONTROLLER_H

#include <boost/bind.hpp>

#include "lost/common/DisplayAttributes.h"
#include "lost/camera/Camera.h"
#include "lost/camera/mathlib.h"
#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"
#include "lost/event/EventDispatcher.h"
#include "lost/application/Timer.h"
#include "lost/application/TimerEvent.h"
#include "lost/application/MouseEvent.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/KeySym.h"
#include "lost/application/TouchEvent.h"

namespace lost
{
  namespace camera
  {
    struct CameraController
    {
      Camera camera;
      
      float                      fovX;
      common::DisplayAttributes& display;
      math::Vec2                 depth;
      math::Vec3                 acceleration;
      math::Vec3                 velocity;
      
      math::Vec2 mousePos;
      Vector3 direction;
      
      application::Timer cameraTimer;
      
      
      CameraController(common::DisplayAttributes& inDisplay, event::EventDispatcher& inDispatcher)
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
        camera.setPosition(Vector3(0.0f, 0.0f, 1.0f));
        camera.setAcceleration(Vector3(acceleration.x, acceleration.y, acceleration.z));
        camera.setVelocity(Vector3(velocity.x, velocity.y, velocity.z));
        
        inDispatcher.addEventListener(application::KeyEvent::KEY_DOWN(), event::receive<application::KeyEvent>(boost::bind(&CameraController::keyPressed, this, _1)));
        inDispatcher.addEventListener(application::KeyEvent::KEY_UP(), event::receive<application::KeyEvent>(boost::bind(&CameraController::keyPressed, this, _1)));
        inDispatcher.addEventListener(application::MouseEvent::MOUSE_MOVE(), event::receive<application::MouseEvent>(boost::bind(&CameraController::mouseMoved, this, _1)));

        inDispatcher.addEventListener(application::TouchEvent::TOUCHES_BEGAN(), event::receive<application::TouchEvent>(bind(&CameraController::touches, this, _1)));
        inDispatcher.addEventListener(application::TouchEvent::TOUCHES_MOVED(), event::receive<application::TouchEvent>(bind(&CameraController::touches, this, _1)));
        inDispatcher.addEventListener(application::TouchEvent::TOUCHES_ENDED(), event::receive<application::TouchEvent>(bind(&CameraController::touches, this, _1)));
        inDispatcher.addEventListener(application::TouchEvent::TOUCHES_CANCELLED(), event::receive<application::TouchEvent>(bind(&CameraController::touches, this, _1)));
        
        cameraTimer.addEventListener(application::TimerEvent::TIMER_FIRED(), event::receive<application::TimerEvent>(boost::bind(&CameraController::updateCamera, this, _1)));
      }
      
      void touches(boost::shared_ptr<application::TouchEvent> event)
      {
        if (event->type == "touchesMoved" && event->touches.size() > 0)
        {
          DOUT("CameraController::touches(" << event->type << ")");
          
          static bool       moveInitialized = false;
          static math::Vec2 position = math::Vec2(0,0);
          
          if (!moveInitialized)
          {
            moveInitialized = true;
            position        = event->touches[0]->location;
          }
          else
          {
            float dx = -(event->touches[0]->location.x - position.x);
            float dy = event->touches[0]->location.y - position.y;
            
            camera.rotateSmoothly(dx, dy, 0.0f);
            DOUT("old pos: " << position << ", new pos: " << event->touches[0]->location);
            position = event->touches[0]->location;
          }
        }
      }
      
      void updateCamera(boost::shared_ptr<application::TimerEvent> event)
      {
        camera.updatePosition(direction, event->passedSec);
      }
      
      void mouseMoved(boost::shared_ptr<application::MouseEvent> event)
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
      
      void keyPressed(boost::shared_ptr<application::KeyEvent> event)
      {
        Vector3 velocity = camera.getCurrentVelocity();
        direction = Vector3(0.0f, 0.0f, 0.0f);
        
        if (event->pressed)
        {
          switch (event->key)
          {
            case application::K_NUMPAD_PLUS:
              camera.setRotationSpeed(camera.getRotationSpeed() + 0.01f);
              if (camera.getRotationSpeed() > 1.0f)
                camera.setRotationSpeed(1.0f);
              break;
              
            case application::K_NUMPAD_MINUS:            
              camera.setRotationSpeed(camera.getRotationSpeed() - 0.01f);
              if (camera.getRotationSpeed() <= 0.0f)
                camera.setRotationSpeed(0.01f);
              break;
              
            case application::K_BACKSPACE:
              if (event->pressed) camera.undoRoll();
              break;
              
            case application::K_W:
              if (!event->repeat) camera.setCurrentVelocity(velocity.x, velocity.y, 0.0f);
              direction.z += 1.0f;
              break;
              
            case application::K_S:
              if (!event->repeat) camera.setCurrentVelocity(velocity.x, velocity.y, 0.0f);
              direction.z -= 1.0f;
              break;
              
            case application::K_A:
              if (!event->repeat) camera.setCurrentVelocity(0.0f, velocity.y, velocity.z);
              direction.x -= 1.0f;
              break;
              
            case application::K_D:
              if (!event->repeat) camera.setCurrentVelocity(0.0f, velocity.y, velocity.z);
              direction.x += 1.0f;
              break;
              
            case application::K_Q:
              if (!event->repeat) camera.setCurrentVelocity(velocity.x, 0.0f, velocity.z);
              direction.y -= 1.0f;
              break;
              
            case application::K_E:
              if (!event->repeat) camera.setCurrentVelocity(velocity.x, 0.0f, velocity.z);
              direction.y += 1.0f;
              break;
          }
        }
      }
    };
    
  }
}

#endif

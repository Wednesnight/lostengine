#ifndef FUNKYFRACTALS_H
#define FUNKYFRACTALS_H

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

#include "lost/gl/gl.h"
#include "lost/gl/Draw.h"
#include "lost/gl/Utils.h"
#include "lost/common/Color.h"
#include "lost/math/Vec2.h"
#include "lost/application/TimerEvent.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/ResizeEvent.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/common/DisplayAttributes.h"
#include "lost/application/KeySym.h"
#include "lost/common/FpsMeter.h"
#include "lost/lsystem/LSystem.h"
#include "lost/gl/ShaderHelper.h"

using namespace boost;
using namespace lost;
using namespace lost::application;
using namespace lost::event;

struct Line
{
  lost::math::Vec3 start;
  lost::math::Vec3 end;
  Line(const lost::math::Vec3& inStart, const lost::math::Vec3& inEnd)
  : start(inStart), end(inEnd)
  {
  }

  void draw(lost::math::Vec3 offset)
  {
    glColor3f(1, 1, 1);
    glutSolidCube(1);
/*
    glBegin(GL_LINES);
      glVertex3f(start.x+offset.x, start.y+offset.y, start.z+offset.z);
      glVertex3f(end.x+offset.x, end.y+offset.y, end.z+offset.z);
    glEnd();
*/
  }
};

struct FunkyFractals
{
  lost::common::FpsMeter  fpsMeter;
  lost::math::Vec3        eye;
  lost::math::Vec3        at;
  lost::math::Vec3        up;
  float                   fovy;
  float                   znear;
  float                   zfar;
  boost::shared_ptr<lost::gl::ShaderProgram>      lightingShader;

  boost::signal<void(void)>        quit;

  lost::lsystem::LSystem                         lsystem;
  boost::shared_ptr<lost::lsystem::LSystemState> state;

  shared_ptr<Timer> t1;
  
  FunkyFractals()
  : eye(50,50,100),
    at(0,50,0),
    up(0,1,0),
    fovy(90),
    znear(1),
    zfar(1000)
  {
  }

  void init(shared_ptr<ApplicationEvent> event)
  {
    appInstance->addEventListener(KeyEvent::KEY_UP(), receive<KeyEvent>(bind(&FunkyFractals::keyboard, this, _1)));
    appInstance->addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&FunkyFractals::keyboard, this, _1)));
    appInstance->addEventListener(ResizeEvent::MAIN_WINDOW_RESIZE(), receive<ResizeEvent>(bind(&FunkyFractals::resetViewPort, this, _1)));
    
    quit.connect(boost::bind(&Application::quit, appInstance));
    t1.reset(new Timer("redrawTimer", 0.015));
    t1->addEventListener(TimerEvent::TIMER_FIRED(), receive<TimerEvent>(bind(&FunkyFractals::redraw, this, _1)));

    state = appInstance->config["state"].as<boost::shared_ptr<lost::lsystem::LSystemState> >();
    
    lightingShaderInit();
  }

  void lightingShaderInit()
  {
    lightingShader = lost::gl::loadShader(appInstance->loader, "lighting");
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
    (*lightingShader)["LightPosition"] = lost::math::Vec3(0,5,-5);
    (*lightingShader)["LightColor"]    = lost::common::Color(1,1,1);
    (*lightingShader)["EyePosition"]   = eye;
    (*lightingShader)["Specular"]      = lost::common::Color(1,1,1);
    (*lightingShader)["Ambient"]       = lost::common::Color(1,.1,.1);
    (*lightingShader)["SurfaceColor"]  = lost::common::Color(1,1,.1);
    (*lightingShader)["Kd"]            = 0.8f;
    lightingShader->disable();
  }

  void resetViewPort(shared_ptr<ResizeEvent> event)
  {
    glViewport(0, 0, event->width-1, event->height-1);GLDEBUG;
  }

  void keyboard(shared_ptr<KeyEvent> event)
  {
    std::string input;
    switch (event->key)
    {
      case K_ESCAPE :
        if (!event->pressed)
        {
          quit();
        }
        break;
    }
  }

  void redraw(shared_ptr<TimerEvent> event)
  {
    boost::shared_ptr<lost::gl::State> newState = appInstance->context->copyState();
    newState->depthTest = true;
    newState->texture2D = false;
    newState->vertexArray = true;
    appInstance->context->pushState(newState);
    glClearColor( 0.0, 0.0, 0.0, 0.0 );GLDEBUG;
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );GLDEBUG;

    lost::gl::utils::set3DProjection(appInstance->displayAttributes.width, appInstance->displayAttributes.height, eye, at, up, fovy, znear, zfar);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    lightingShader->enable();
    lost::math::Matrix matrix;
    matrix.initTranslation(lost::math::Vec3(0,0,0));
    glLoadMatrixf(matrix.m);GLDEBUG;
    lsystem.walk(state, matrix, boost::bind(&FunkyFractals::renderLSystemNode, this, _1, _2));
    lightingShader->disable();

    lost::gl::utils::set2DProjection(lost::math::Vec2(0,0), lost::math::Vec2(appInstance->displayAttributes.width, appInstance->displayAttributes.height));
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    fpsMeter.render(appInstance->displayAttributes.width - fpsMeter.width, 0, event->passedSec);

    appInstance->context->popState();

    glfwSwapBuffers();
  }

  void renderLSystemNode( lost::math::Matrix& matrix, lost::lsystem::LSystemResult& result )
  {
    if (result.drawable)
    {
      Line l(lost::math::Vec3(0, 0, 0), lost::math::Vec3(0, 2, 0));
      l.draw(lost::math::Vec3(0, 0, 0));

      lost::math::Matrix matrix;
      matrix.initTranslation(lost::math::Vec3(0, 2, 0));
      result.matrix = result.matrix * matrix;
    }
    else
    {
      glLoadMatrixf(result.matrix.m);GLDEBUG;
    }
  }

};

#endif

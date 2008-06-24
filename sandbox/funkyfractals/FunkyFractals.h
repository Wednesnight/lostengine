#ifndef FUNKYFRACTALS_H
#define FUNKYFRACTALS_H

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

#include "lost/gl/gl.h"
#include "lost/gl/Draw.h"
#include "lost/gl/Utils.h"
#include "lost/common/Color.h"
#include "lost/math/Vec2.h"
#include "lost/application/Timer.h"
#include "lost/common/DisplayAttributes.h"
#include "lost/engine/KeySym.h"
#include "lost/common/FpsMeter.h"
#include "lost/lsystem/LSystem.h"
#include "lost/gl/ShaderHelper.h"

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

  lost::common::DisplayAttributes& displayAttributes;
  boost::signal<void(void)>        quit;

  lost::lsystem::LSystem                         lsystem;
  boost::shared_ptr<lost::lsystem::LSystemState> state;

  lost::lua::State& interpreter;
  
  FunkyFractals(lost::common::DisplayAttributes& inDisplayAttributes, lost::lua::State& inInterpreter)
  : eye(50,50,100),
    at(0,50,0),
    up(0,1,0),
    fovy(90),
    znear(1),
    zfar(1000),
    displayAttributes(inDisplayAttributes),
    interpreter(inInterpreter)
  {
/*
    std::map<char, std::string> variableMap;
    variableMap['F'] = "FzXFZxFzXFzXF";
    state.reset(new lost::lsystem::LSystemState("FzFzFzF", variableMap, lost::math::Vec3(5,0,90)));
    state->reset();
    lsystem.advance(state, 6);
*/
    interpreter.doResourceFile("funkyfractals.lua");      
    state = luabind::object_cast<boost::shared_ptr<lost::lsystem::LSystemState> >(luabind::globals(interpreter)["state"]);

    lightingShaderInit();
  }

  void lightingShaderInit()
  {
    lightingShader = lost::gl::loadShader("lighting");
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

  void resetViewPort(int width, int height)
  {
    glViewport(0, 0, width-1, height-1);GLDEBUG;
    lost::gl::utils::set3DProjection(displayAttributes.width, displayAttributes.height, eye, at, up, fovy, znear, zfar);
  }

  void keyboard( const lost::event::KeyEvent& inEvent )
  {
    std::string input;
    switch (inEvent.key)
    {
      case lost::engine::K_ESCAPE :
        if (!inEvent.pressed)
        {
          quit();
        }
        break;
      case lost::engine::K_F6 :
        std::cin >> input;
        interpreter.doString(input);
        break;
      default :
      break;
    }
  }

  void redraw(double deltaSec, lost::application::Timer* timer)
  {
    glClearColor( 0.0, 0.0, 0.0, 0.0 );GLDEBUG;
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );GLDEBUG;

    glEnable(GL_DEPTH_TEST);

    lost::gl::utils::set3DProjection(displayAttributes.width, displayAttributes.height, eye, at, up, fovy, znear, zfar);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    lightingShader->enable();
    lost::math::Matrix matrix;
    matrix.initTranslation(lost::math::Vec3(0,0,0));
    glLoadMatrixf(matrix.m);GLDEBUG;
    lsystem.walk(state, matrix, boost::bind(&FunkyFractals::renderLSystemNode, this, _1, _2));
    lightingShader->disable();

    lost::gl::utils::set2DProjection(lost::math::Vec2(0,0), lost::math::Vec2(displayAttributes.width, displayAttributes.height));
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    fpsMeter.render(displayAttributes.width - fpsMeter.width, 0, deltaSec);
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

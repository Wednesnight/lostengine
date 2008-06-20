#ifndef SHAD0R_H
#define SHAD0R_H

#include "lost/gl/gl.h"
#include "lost/gl/Draw.h"
#include "lost/gl/Utils.h"
#include "lost/common/Color.h"
#include "lost/math/Vec2.h"
#include "lost/application/Timer.h"
#include "lost/common/DisplayAttributes.h"
#include "lost/engine/KeySym.h"
#include "lost/common/FpsMeter.h"
#include "lost/gl/VertexShader.h"
#include "lost/gl/FragmentShader.h"
#include "lost/gl/ShaderProgram.h"
#include <boost/shared_ptr.hpp>
#include <fstream>
#include "lost/common/FileHelpers.h"
#include "lost/gl/Shader.h"
#include "lost/gl/ShaderHelper.h"

struct Shad0r
{
  lost::common::FpsMeter fpsMeter;
  boost::shared_ptr<lost::gl::ShaderProgram>  shaderProgram;

  Shad0r(lost::common::DisplayAttributes& inDisplayAttributes)
  : displayAttributes(inDisplayAttributes)
  {
    init();
  }


  void init()
  {
    resetViewPort(displayAttributes.width, displayAttributes.height);

    shaderProgram = lost::gl::loadShader("temperature");
    
    shaderProgram->validate();
    DOUT("shader validated: "<<shaderProgram->validated());
    DOUT("validation log: "<<shaderProgram->log());
    
    shaderProgram->enable();
    (*shaderProgram)["CoolestTemp"] = .2f;
    (*shaderProgram)["TempRange"] = .7f;
    (*shaderProgram)["HottestColor"] = lost::common::redColor;
    (*shaderProgram)["CoolestColor"] = lost::common::blueColor;
    shaderProgram->disable();
  }

  void resetViewPort(int width, int height)
  {
    glViewport(0, 0, width, height);GLDEBUG;
    lost::gl::utils::set2DProjection(lost::math::Vec2(0, 0), lost::math::Vec2(width, height));GLDEBUG;
  }

  void keyboard( const lost::event::KeyEvent& inEvent )
  {
    switch (inEvent.key)
    {
      case lost::engine::K_ESCAPE :
        if (!inEvent.pressed)
        {
          quit();
        }
        break;
        default :
        break;
    }
  }

  void redraw(double deltaSec, lost::application::Timer* timer)
  {
    glGetError();
    glEnable(GL_BLEND);GLDEBUG;
    glDisable(GL_DEPTH_TEST);GLDEBUG;
    glDisable(GL_TEXTURE_2D);GLDEBUG;
    glClearColor( 0.0, 0.0, 0.0, 0.0 );GLDEBUG;
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );GLDEBUG;

    shaderProgram->enable();
    lost::gl::setColor(lost::common::whiteColor);
    glBegin(GL_LINES);
    (*shaderProgram)["VertexTemp"] = 0.3f;
    glVertex2f(0,0);
    (*shaderProgram)["VertexTemp"] = 0.7f;
    glVertex2f(displayAttributes.width-1, displayAttributes.height-1);
    glEnd();
    shaderProgram->disable();

    fpsMeter.render(1, 0, deltaSec);
    glfwSwapBuffers();
  }

  lost::common::DisplayAttributes& displayAttributes;
  boost::signal<void(void)> quit;
};

#endif

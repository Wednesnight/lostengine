#ifndef MATRIX0R_H
#define MATRIX0R_H

#include "lost/gl/gl.h"
#include "lost/gl/Draw.h"
#include "lost/gl/Utils.h"
#include "lost/common/Color.h"
#include "lost/math/Vec2.h"
#include "lost/application/Timer.h"
#include "lost/common/DisplayAttributes.h"
#include "lost/engine/KeySym.h"
#include "lost/common/FpsMeter.h"
#include "lost/math/Vec4.h"
#include "lost/math/Matrix.h"

struct Line
{
  lost::math::Vec4 start;
  lost::math::Vec4 end;
  Line(const lost::math::Vec4& inStart, const lost::math::Vec4& inEnd)
  : start(inStart), end(inEnd)
  {
  }

  void draw(lost::math::Vec2 offset)
  {
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
      glVertex2f(start.x+offset.x, start.y+offset.y);
      glVertex2f(end.x+offset.x, end.y+offset.y);
    glEnd();
  }
};

struct Matrix0r
{
  lost::common::FpsMeter  fpsMeter;

  Matrix0r(lost::common::DisplayAttributes& inDisplayAttributes)
  : displayAttributes(inDisplayAttributes)
  {
  }


  void resetViewPort(int width, int height)
  {
    glViewport(0, 0, width-1, height-1);GLDEBUG;
    lost::gl::utils::set2DProjection(lost::math::Vec2(0,0), lost::math::Vec2(displayAttributes.width, displayAttributes.height));
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
    glClearColor( 0.0, 0.0, 0.0, 0.0 );GLDEBUG;
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );GLDEBUG;

    lost::gl::utils::set2DProjection(lost::math::Vec2(0,0), lost::math::Vec2(displayAttributes.width, displayAttributes.height));

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    lost::math::Matrix trans;
    trans.initTranslation(lost::math::Vec3(320,240,0));
    glLoadMatrixf(trans.m);

    lost::math::Vec2 offset(0, 0);
    Line l(lost::math::Vec4(0, 0, 0, 1), lost::math::Vec4(0, 100, 0, 1));
    l.draw(offset);

    lost::math::Matrix rotz;
    rotz.initRotateZ(45);
    rotz = trans * rotz;
    glLoadMatrixf(rotz.m);

    l.draw(offset);

    trans.initTranslation(lost::math::Vec3(10,10,0));
    trans = rotz * trans;
    glLoadMatrixf(trans.m);

    l.draw(offset);

    rotz.initRotateZ(45);
    rotz = trans * rotz;
    glLoadMatrixf(rotz.m);

    l.draw(offset);

    trans.initTranslation(lost::math::Vec3(10,10,0));
    trans = rotz * trans;
    glLoadMatrixf(trans.m);

    l.draw(offset);

    rotz.initRotateZ(45);
    rotz = trans * rotz;
    glLoadMatrixf(rotz.m);

    l.draw(offset);

    trans.initTranslation(lost::math::Vec3(10,10,0));
    trans = rotz * trans;
    glLoadMatrixf(trans.m);

    l.draw(offset);

    rotz.initRotateZ(45);
    rotz = trans * rotz;
    glLoadMatrixf(rotz.m);

    l.draw(offset);

    trans.initTranslation(lost::math::Vec3(10,10,0));
    trans = rotz * trans;
    glLoadMatrixf(trans.m);

    l.draw(offset);

    rotz.initRotateZ(45);
    rotz = trans * rotz;
    glLoadMatrixf(rotz.m);

    l.draw(offset);

    trans.initTranslation(lost::math::Vec3(10,10,0));
    trans = rotz * trans;
    glLoadMatrixf(trans.m);

    l.draw(offset);

    rotz.initRotateZ(45);
    rotz = trans * rotz;
    glLoadMatrixf(rotz.m);

    l.draw(offset);

    trans.initTranslation(lost::math::Vec3(10,10,0));
    trans = rotz * trans;
    glLoadMatrixf(trans.m);

    l.draw(offset);

    rotz.initRotateZ(45);
    rotz = trans * rotz;
    glLoadMatrixf(rotz.m);

    l.draw(offset);

    trans.initTranslation(lost::math::Vec3(10,10,0));
    trans = rotz * trans;
    glLoadMatrixf(trans.m);

    l.draw(offset);

    rotz.initRotateZ(45);
    rotz = trans * rotz;
    glLoadMatrixf(rotz.m);

    l.draw(offset);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    fpsMeter.render(displayAttributes.width - fpsMeter.width, 0, deltaSec);
    glfwSwapBuffers();
  }

  lost::common::DisplayAttributes& displayAttributes;
  boost::signal<void(void)> quit;
};

#endif

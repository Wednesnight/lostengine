#ifndef MOUSEFUXX0R_H
#define MOUSEFUXX0R_H

#include "Visual.h"
#include "lost/DisplayAttributes.h"
#include "lost/math/Vec3.h"
#include "lost/math/Vec2.h"
#include "lost/math/Generator.h"
#include "lost/InputSym.h"

struct MouseFuxx0r : public Visual
{
  bool                           doDraw;
  const lost::DisplayAttributes& display;
  lost::math::Vec2               origin;

  MouseFuxx0r( const lost::DisplayAttributes& attr ) : display(attr)
  {
    doDraw = false;
  }

  void redraw(double timeSinceLastCallSec)
  {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    lost::gl::Utils::set2DProjection(lost::math::Vec2(0, 0), lost::math::Vec2(display.width, display.height));

    int x, y;
    glfwGetMousePos( &x, &y );
    y = display.height - y;
    glColor3f( 1, 1, 1 );
    glBegin(GL_LINES);
    glVertex2f(x-5, y);
    glVertex2f(x+5, y);
    glVertex2f(x, y-5);
    glVertex2f(x, y+5);
    glEnd();

    if (doDraw)
    {
      glColor3f( 1, 0, 0 );
      glBegin(GL_LINES);
      glVertex2f(origin.x,origin.y);
      glVertex2f(x, y);
      glEnd();
    }
  }

  void mouseButton( int button, bool pressed, lost::math::Vec2 point )
  {
    if (button == KEY_MOUSE_LEFT)
    {
      float x = point.x;
      float y = display.height - point.y;
      if (pressed)
      {
        origin.x = x;
        origin.y = y;
        doDraw   = true;
      }
      else
      {
        doDraw = false;
      }
    }
  }


};

#endif

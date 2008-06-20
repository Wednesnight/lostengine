#ifndef ZOMBONG_H
#define ZOMBONG_H

#include "Visual.h"
#include "lost/DisplayAttributes.h"
#include "lost/math/Vec3.h"
#include "lost/math/Vec2.h"
#include "lost/math/Generator.h"
#include "lost/gl/Utils.h"

struct Zombong : public Visual
{
  // display attributes
  const lost::DisplayAttributes& display;
  // state
  bool running;
  // positions
  GLfloat          positionLeft;
  GLfloat          positionRight;
  lost::math::Vec2 positionPoint;
  lost::math::Vec2 factorPoint;
  // score
  int scoreLeft;
  int scoreRight;

  Zombong( const lost::DisplayAttributes& attr ) : display(attr)
  {
    running         = false;
    positionLeft    = display.height/2;
    positionRight   = display.height/2;
    positionPoint.x = 25;
    positionPoint.y = display.height/2;
    factorPoint.x   = 15;
    factorPoint.y   = 0;
    scoreLeft       = 0;
    scoreRight      = 0;
  }

  void updatePositions()
  {
    if (glfwGetKey( 'W' ) == GLFW_PRESS && positionLeft < display.height - 50)
    {
      positionLeft += 10;
    }
    if (glfwGetKey( 'S' ) == GLFW_PRESS && positionLeft > 50)
    {
      positionLeft -= 10;
    }
    if (glfwGetKey( GLFW_KEY_UP ) == GLFW_PRESS && positionRight < display.height - 50)
    {
      positionRight += 10;
    }
    if (glfwGetKey( GLFW_KEY_DOWN ) == GLFW_PRESS && positionRight > 50)
    {
      positionRight -= 10;
    }

    if (running)
    {
      positionPoint.x += factorPoint.x;
      if (positionPoint.x >= display.width - 20)
      {
        if (positionPoint.y <= positionRight + 50 && positionPoint.y >= positionRight - 50)
        {
          factorPoint.x = -factorPoint.x;
          factorPoint.y = (positionPoint.y - positionRight) / 5;
        }
        else
        {
          ++scoreLeft;
          running = false;
        }
      }
      else if (positionPoint.x <= 20)
      {
        if (positionPoint.y <= positionLeft + 50 && positionPoint.y >= positionLeft - 50)
        {
          factorPoint.x = -factorPoint.x;
          factorPoint.y = (positionPoint.y - positionLeft) / 5;
        }
        else
        {
          ++scoreRight;
          running = false;
        }
      }
      positionPoint.y += factorPoint.y;
      if (positionPoint.y >= display.height - 5 || positionPoint.y <= 5)
      {
        factorPoint.y = -factorPoint.y;
      }
    }
  }

  void reset()
  {
    positionPoint.x = 25;
    positionPoint.y = display.height/2;
    factorPoint.x   = 15;
    factorPoint.y   = 0;
  }

  void redraw(double timeSinceLastCallSec)
  {
    if (!running)
    {
      running = (glfwGetKey( GLFW_KEY_SPACE ) == GLFW_PRESS);
      if (running)
        reset();
    }

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    lost::gl::Utils::set2DProjection(lost::math::Vec2(0, 0), lost::math::Vec2(display.width, display.height));
  
    updatePositions();
  
    glColor3f( 1, 1, 1 );
    glLineWidth( 1.0 );
    glBegin(GL_LINES);
      glVertex2f(display.width/2, 0);
      glVertex2f(display.width/2, display.height);
    glEnd();    
    glLineWidth( 5.0 );
    glBegin(GL_LINES);
      glVertex2f(5, positionLeft - 50);
      glVertex2f(5, positionLeft + 50);
      glVertex2f(display.width - 5, positionRight - 50);
      glVertex2f(display.width - 5, positionRight + 50);
    glEnd();    
    glPointSize( 10.0 );
    glBegin(GL_POINTS);
      glVertex2f(positionPoint.x, positionPoint.y);
    glEnd();

    // reset to defaults
    glLineWidth( 1.0 );
    glPointSize( 1.0 );

    glColor3f( 0, 1, 0 );
    lost::gl::Utils::drawString( 5, display.height - 25, boost::lexical_cast<std::string>(scoreLeft) );
    lost::gl::Utils::drawString( display.width - 15, display.height - 25, boost::lexical_cast<std::string>(scoreRight) );
    glColor3f( 0, 0, 1 );
    lost::gl::Utils::drawString( 5, 5, "W - Up   S - Down" );
    lost::gl::Utils::drawString( display.width - 175, 5, "UP - Up   DOWN - Down" );
    if (!running)
    {
      glColor3f( 1, 0, 0 );
      lost::gl::Utils::drawString( (display.width/2) - 45, 5, "Press SPACE" );
    }
  }
  
};

#endif

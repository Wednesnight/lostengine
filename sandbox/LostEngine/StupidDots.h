#ifndef STUPIDDOTS_H
#define STUPIDDOTS_H

#include "Visual.h"
#include "lost/DisplayAttributes.h"
#include "lost/math/Vec3.h"
#include "lost/math/Vec2.h"
#include "lost/math/Generator.h"

struct StupidDots : public Visual
{
  float cury;
  float curx;
  float r;
  float g;
  float b;
  float pointsize;

  lost::math::generator::sine rgen;
  lost::math::generator::sine ggen;
  lost::math::generator::sine bgen;
  lost::math::generator::sine pointsizegen;
  lost::math::generator::sine xgen;
  lost::math::generator::cosine ygen;

  const lost::DisplayAttributes& display;
  
  StupidDots(const lost::DisplayAttributes& attr) : display(attr)
  {
    float centerx = attr.width / 2;
    float centery = attr.height / 2;
    float radiusx = attr.width / 2;
    float radiusy = attr.height / 2;
    
    
    xgen.target(curx);
    xgen.period = 1;
    xgen.periodoffset = 0;
    xgen.ampoffset = centerx;
    xgen.absamplitude = radiusx;
    
    ygen.target(cury);
    ygen.period = .7;
    ygen.periodoffset = 0;
    ygen.ampoffset = centery;
    ygen.absamplitude = radiusy;
    
    rgen.target(r);
    rgen.period = 1;
    rgen.periodoffset = 0;
    rgen.ampoffset = .5;
    rgen.absamplitude = .5;
    
    ggen.target(g);
    ggen.period = 1;
    ggen.periodoffset = 0;
    ggen.ampoffset = .5;
    ggen.absamplitude = .5;
    
    bgen.target(b);
    bgen.period = 1;
    bgen.periodoffset = 0;
    bgen.ampoffset = .5;
    bgen.absamplitude = .5;
    
    pointsizegen.target(pointsize);
    pointsizegen.period = .98;
    pointsizegen.periodoffset = 0;
    pointsizegen.ampoffset = 40;
    pointsizegen.absamplitude = 10;    

    updateData(0); // reset targets to initial values
  }
  
  void updateData(double timeSinceLastCallSec)
  {
    xgen.update(timeSinceLastCallSec);
    ygen.update(timeSinceLastCallSec);
    rgen.update(timeSinceLastCallSec);
    bgen.update(timeSinceLastCallSec);
    ggen.update(timeSinceLastCallSec);
    pointsizegen.update(timeSinceLastCallSec);
  }

  void redraw(double timeSinceLastCallSec)
  {    
    updateData(timeSinceLastCallSec);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    lost::gl::Utils::set2DProjection(lost::math::Vec2(0, 0), lost::math::Vec2(display.width, display.height));
    
    glColor3f(r, g, b);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glPointSize(pointsize);
    glBegin(GL_POINTS);
    glVertex2f(curx, cury);
    glEnd();    
    
    
    glColor3f(1,1,1);
    glBegin(GL_LINE);
      glVertex2f(0,0);
      glVertex2f(200, 200);
    glEnd();
  }
  
};

#endif

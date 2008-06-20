#ifndef COCK_H
#define COCK_H

#include "Visual.h"
#include <vector>

struct Cock : public Visual
{
  struct Line
  {
    lost::math::Vec2 start;
    lost::math::Vec2 end;

    Line() {}
    Line(float sx, float sy, float ex, float ey) : start(sx, sy), end(ex, ey) {}
    Line(const lost::math::Vec2& s, const lost::math::Vec2& e) : start(s), end(e) {}
  };

  void cockLineExplode(const Line& orig, Line& left, Line& right)
  {
    lost::math::Vec2 v = orig.end - orig.start;
    lost::math::Vec2 vp = perpendicular(v);
    normalise(vp);
    lost::math::Vec2 mid = orig.start + (v*0.5);
    lost::math::Vec2 center = mid + vp*len(v);

    left = Line(orig.start,center);
    right = Line(center, orig.end);
  }

  void subdivide(const Line& orig, Line& left, Line& centerleft, Line& centerright, Line& right)
  {
    lost::math::Vec2 v = orig.end - orig.start;
    Line center;

    lost::math::Vec2 p1 = orig.start + v*.33333;
    lost::math::Vec2 p2 = orig.start + v*.66666;

    left = Line(orig.start, p1);
    center = Line(p1, p2);
    right = Line(p2, orig.end);

    cockLineExplode(center, centerleft, centerright);
  }

  std::vector<Line> fullFrontalCockLineExplode(const std::vector<Line>& orig)
  {
    std::vector<Line> result;

    Line left;
    Line centerleft;
    Line centerright;
    Line right;

    for(unsigned long i= 0; i<orig.size(); ++i)
    {
      subdivide(orig[i], left, centerleft, centerright, right);
      result.push_back(left);
      result.push_back(centerleft);
      result.push_back(centerright);
      result.push_back(right);
    }

    return result;
  }

  std::vector<Line> lines;

  const lost::DisplayAttributes& display;

  Cock(const lost::DisplayAttributes& attr) : display(attr)
  {
    //lines.push_back(Line(0, 0, display.width, display.height/2));

    float xinset = 20;
    float yinset = 200;

    lost::math::Vec2 topleft(xinset, display.height-yinset);
    lost::math::Vec2 topright(display.width-xinset, display.height-yinset);
    lost::math::Vec2 bottom(display.width/2, 0);

    lines.push_back(Line(bottom, topleft));
    lines.push_back(Line(topleft, topright));
    lines.push_back(Line(topright, bottom));

    unsigned long iterations = 5;
    for(unsigned long i= 0; i<iterations; ++i)
    {
      lines = fullFrontalCockLineExplode(lines);
    }
  }

  // avoid compiler warning
  virtual ~Cock() {}

  void drawLine(const Line& line)
  {
    glBegin(GL_LINES);
    glVertex2f(line.start.x,line.start.y);
    glVertex2f(line.end.x, line.end.y);
    glEnd();
  }

  void redraw(double timeSinceLastCallSec)
  {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    lost::gl::Utils::set2DProjection(lost::math::Vec2(0, 0), lost::math::Vec2(display.width, display.height));

    glColor3f(1,1,1);
    for(unsigned long i=0; i<lines.size(); ++i)
    {
      drawLine(lines[i]);
    }
  }
};

#endif

#ifndef LOST_MATH_BEZIER_H
#define LOST_MATH_BEZIER_H

#include <vector>
#include "lost/math/Vec2.h"
#include "lost/math/lmath.h"
#include "lost/platform/shared_ptr.h"

namespace lost
{
  namespace math
  {

    struct Bezier;
    typedef shared_ptr<Bezier> BezierPtr;

    struct Bezier
    {
    public:
      std::vector<Vec2> points;

      // add methods like scale, ...
    };

    struct LinearBezier : public Bezier
    {
    private:
      LinearBezier(const Vec2& p0, const Vec2& p1, const unsigned int steps = 10)
      {
        double stepSize = 1.0 / steps;
        double t = 0.0;
        Vec2 point;
        while (t <= 1.0)
        {
          point.x = (1 - t) * p0.x + t * p1.x;
          point.y = (1 - t) * p0.y + t * p1.y;
          points.push_back(point);
          t += stepSize;
        }
      }

    public:
      static BezierPtr create(const Vec2& p0, const Vec2& p1, const unsigned int steps = 10)
      {
        return BezierPtr(new LinearBezier(p0, p1, steps));
      }

    };

    struct QuadraticBezier : public Bezier
    {
    private:
      QuadraticBezier(const Vec2& p0, const Vec2& p1, const Vec2& p2, const unsigned int steps = 10)
      {
        double stepSize = 1.0 / steps;
        double t = 0.0;
        Vec2 point;
        while (t <= 1.0)
        {
          point.x = ((1 - t) * (1 - t)) * p0.x + (2*t) * (1-t) * p1.x + (t*t) * p2.x;
          point.y = ((1 - t) * (1 - t)) * p0.y + (2*t) * (1-t) * p1.y + (t*t) * p2.y;
          points.push_back(point);
          t += stepSize;
        }
      }
      
    public:
      static BezierPtr create(const Vec2& p0, const Vec2& p1, const Vec2& p2, const unsigned int steps = 10)
      {
        return BezierPtr(new QuadraticBezier(p0, p1, p2, steps));
      }
      
    };
    
    struct CubicBezier : public Bezier
    {
    private:
      CubicBezier(const Vec2& p0, const Vec2& p1, const Vec2& p2, const Vec2& p3, const unsigned int steps = 10)
      {
        double stepSize = 1.0 / steps;
        double t = 0.0;
        Vec2 point;
        while (t <= 1.0)
        {
          point.x = (-p0.x + 3*p1.x - 3*p2.x + p3.x) * (t*t*t) + (3*p0.x - 6*p1.x + 3*p2.x) * (t*t) + (-3*p0.x + 3*p1.x) * t + p0.x;
          point.y = (-p0.y + 3*p1.y - 3*p2.y + p3.y) * (t*t*t) + (3*p0.y - 6*p1.y + 3*p2.y) * (t*t) + (-3*p0.y + 3*p1.y) * t + p0.y;
          points.push_back(point);
          t += stepSize;
        }
      }
      
    public:
      static BezierPtr create(const Vec2& p0, const Vec2& p1, const Vec2& p2, const Vec2& p3, const unsigned int steps = 10)
      {
        return BezierPtr(new CubicBezier(p0, p1, p2, p3, steps));
      }
      
    };
    
  }
}

#endif

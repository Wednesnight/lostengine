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
    struct LinearBezier;
    typedef shared_ptr<LinearBezier> LinearBezierPtr;
    struct QuadraticBezier;
    typedef shared_ptr<QuadraticBezier> QuadraticBezierPtr;
    struct CubicBezier;
    typedef shared_ptr<CubicBezier> CubicBezierPtr;

    struct Bezier
    {
    public:
      std::vector<Vec2> points;

      static double defaultSteps()
      {
        return 10;
      }

      // add methods like scale, ...
    };

    struct LinearBezier : public Bezier
    {
    private:
      LinearBezier(const Vec2& p0, const Vec2& p1, const unsigned int steps = Bezier::defaultSteps())
      {
        update(p0, p1, steps);
      }

    public:
      static LinearBezierPtr create(const Vec2& p0, const Vec2& p1, const unsigned int steps = Bezier::defaultSteps())
      {
        return LinearBezierPtr(new LinearBezier(p0, p1, steps));
      }

      void update(const Vec2& p0, const Vec2& p1, const unsigned int steps = Bezier::defaultSteps())
      {
        points.clear();
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

    };

    struct QuadraticBezier : public Bezier
    {
    private:
      QuadraticBezier(const Vec2& p0, const Vec2& p1, const Vec2& p2, const unsigned int steps = Bezier::defaultSteps())
      {
        update(p0, p1, p2, steps);
      }
      
      QuadraticBezier(const Vec2& corner, const Vec2& direction, const double radius, const unsigned int steps = Bezier::defaultSteps())
      {
        update(corner, direction, radius, steps);
      }

    public:
      static QuadraticBezierPtr create(const Vec2& p0, const Vec2& p1, const Vec2& p2, const unsigned int steps = Bezier::defaultSteps())
      {
        return QuadraticBezierPtr(new QuadraticBezier(p0, p1, p2, steps));
      }
      
      static QuadraticBezierPtr create(const Vec2& corner, const Vec2& direction, const double radius, const unsigned int steps = Bezier::defaultSteps())
      {
        return QuadraticBezierPtr(new QuadraticBezier(corner, direction, radius, steps));
      }
      
      void update(const Vec2& corner, const Vec2& direction, const double radius, const unsigned int steps = Bezier::defaultSteps())
      {
        Vec2 p1 = corner;
        double length = sin(45.0)*radius;
        Vec2 p0(p1.x, p1.y+direction.y*length);
        Vec2 p2(p1.x+direction.x*length, p1.y);
        update(p0, p1, p2, steps);        
      }
      
      void update(const Vec2& p0, const Vec2& p1, const Vec2& p2, const unsigned int steps = Bezier::defaultSteps())
      {
        points.clear();
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
      
    };
    
    struct CubicBezier : public Bezier
    {
    private:
      CubicBezier(const Vec2& p0, const Vec2& p1, const Vec2& p2, const Vec2& p3, const unsigned int steps = Bezier::defaultSteps())
      {
        update(p0, p1, p2, p3, steps);
      }
      
    public:
      static CubicBezierPtr create(const Vec2& p0, const Vec2& p1, const Vec2& p2, const Vec2& p3, const unsigned int steps = Bezier::defaultSteps())
      {
        return CubicBezierPtr(new CubicBezier(p0, p1, p2, p3, steps));
      }
    
      void update(const Vec2& p0, const Vec2& p1, const Vec2& p2, const Vec2& p3, const unsigned int steps = Bezier::defaultSteps())
      {
        points.clear();
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

    };
    
  }
}

#endif

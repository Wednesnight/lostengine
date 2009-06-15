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

      static Vec2* create(Vec2* array, unsigned int offset, const Vec2& p0, const Vec2& p1,
                          const unsigned int steps = Bezier::defaultSteps())
      {
        unsigned int idx = offset;
        double stepSize = 1.0 / steps;
        double t = 0.0;
        while (t <= 1.0)
        {
          array[idx++] = calculate(p0, p1, t);
          t += stepSize;
        }
        return array;
      }
      
      void update(const Vec2& p0, const Vec2& p1, const unsigned int steps = Bezier::defaultSteps())
      {
        points.clear();
        double stepSize = 1.0 / steps;
        double t = 0.0;
        while (t <= 1.0)
        {
          points.push_back(calculate(p0, p1, t));
          t += stepSize;
        }
      }

      static inline Vec2 calculate(const Vec2& p0, const Vec2& p1, const double t)
      {
        Vec2 result;
        result.x = (1 - t) * p0.x + t * p1.x;
        result.y = (1 - t) * p0.y + t * p1.y;
        return result;
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
      
      static Vec2* create(Vec2* array, unsigned int offset, const Vec2& p0, const Vec2& p1, const Vec2& p2,
                          const unsigned int steps = Bezier::defaultSteps())
      {
        unsigned int idx = offset;
        double stepSize = 1.0 / steps;
        double t = 0.0;
        while (t <= 1.0)
        {
          array[idx++] = calculate(p0, p1, p2, t);
          t += stepSize;
        }
        return array;
      }
      
      static Vec2* create(Vec2* array, unsigned int offset, const Vec2& corner, const Vec2& direction, const double radius,
                          const unsigned int steps = Bezier::defaultSteps())
      {
        Vec2 p1 = corner;
        double length = sin(45.0)*radius;
        Vec2 p0;
        Vec2 p2;
        if ((direction.x < 0 && direction.y > 0) || (direction.x > 0 && direction.y < 0))
        {
          p0 = Vec2(p1.x+direction.x*length, p1.y);
          p2 = Vec2(p1.x, p1.y+direction.y*length);
        }
        else
        {
          p0 = Vec2(p1.x, p1.y+direction.y*length);
          p2 = Vec2(p1.x+direction.x*length, p1.y);
        }
        return create(array, offset, p0, p1, p2, steps);
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
        while (t <= 1.0)
        {
          points.push_back(calculate(p0, p1, p2, t));
          t += stepSize;
        }
      }
      
      static inline Vec2 calculate(const Vec2& p0, const Vec2& p1, const Vec2& p2, const double t)
      {
        Vec2 result;
        result.x = ((1 - t) * (1 - t)) * p0.x + (2*t) * (1-t) * p1.x + (t*t) * p2.x;
        result.y = ((1 - t) * (1 - t)) * p0.y + (2*t) * (1-t) * p1.y + (t*t) * p2.y;
        return result;
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
    
      static Vec2* create(Vec2* array, unsigned int offset, const Vec2& p0, const Vec2& p1, const Vec2& p2, const Vec2& p3,
                          const unsigned int steps = Bezier::defaultSteps())
      {
        unsigned int idx = offset;
        double stepSize = 1.0 / steps;
        double t = 0.0;
        while (t <= 1.0)
        {
          array[idx++] = calculate(p0, p1, p2, p3, t);
          t += stepSize;
        }
        return array;
      }
      
      void update(const Vec2& p0, const Vec2& p1, const Vec2& p2, const Vec2& p3, const unsigned int steps = Bezier::defaultSteps())
      {
        points.clear();
        double stepSize = 1.0 / steps;
        double t = 0.0;
        while (t <= 1.0)
        {
          points.push_back(calculate(p0, p1, p2, p3, t));
          t += stepSize;
        }
      }

      static inline Vec2 calculate(const Vec2& p0, const Vec2& p1, const Vec2& p2, const Vec2& p3, const double t)
      {
        Vec2 result;
        result.x = (-p0.x + 3*p1.x - 3*p2.x + p3.x) * (t*t*t) + (3*p0.x - 6*p1.x + 3*p2.x) * (t*t) + (-3*p0.x + 3*p1.x) * t + p0.x;
        result.y = (-p0.y + 3*p1.y - 3*p2.y + p3.y) * (t*t*t) + (3*p0.y - 6*p1.y + 3*p2.y) * (t*t) + (-3*p0.y + 3*p1.y) * t + p0.y;
        return result;
      }

    };
    
  }
}

#endif

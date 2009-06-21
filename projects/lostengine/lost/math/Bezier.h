#ifndef LOST_MATH_BEZIER_H
#define LOST_MATH_BEZIER_H

#include <vector>
#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"
#include "lost/math/lmath.h"
#include "lost/platform/shared_ptr.h"

namespace lost
{
  namespace math
  {
  
    // FIXME: defaultSteps as a member of a templated class is probably unnecessary
    // unless we want to specialize the return value according to the template parameter.
    template<typename VEC>
    struct Bezier
    {
    public:
      std::vector<VEC> points;

      static boost::uint32_t defaultSteps()
      {
        return 10;
      }

      // add methods like scale, ...
    };

    template<typename VEC>
    struct LinearBezier : public Bezier<VEC>
    {
    private:
      LinearBezier(const VEC& p0,
                   const VEC& p1,
                   boost::uint32_t steps = Bezier<VEC>::defaultSteps())
      {
        update(p0, p1, steps);
      }

    public:
      static lost::shared_ptr<LinearBezier<VEC> > create(const VEC& p0,
                                    const VEC& p1,
                                    boost::uint32_t steps = Bezier<VEC>::defaultSteps())
      {
        return lost::shared_ptr<LinearBezier<VEC> >(new LinearBezier<VEC>(p0, p1, steps));
      }

      static VEC* create(VEC* array,
                         boost::uint32_t offset,
                         const VEC& p0,
                         const VEC& p1,
                         boost::uint32_t steps = Bezier<VEC>::defaultSteps())
      {
        boost::uint32_t idx = offset;
        float stepSize = 1.0 / steps;
        float t = 0.0;
        while (t <= 1.0)
        {
          array[idx++] = calculate(p0, p1, t);
          t += stepSize;
        }
        return array;
      }
      
      void update(const VEC& p0,
                  const VEC& p1,
                  boost::uint32_t steps = Bezier<VEC>::defaultSteps())
      {
        Bezier<VEC>::points.clear();
        float stepSize = 1.0 / steps;
        float t = 0.0;
        while (t <= 1.0)
        {
          Bezier<VEC>::points.push_back(calculate(p0, p1, t));
          t += stepSize;
        }
      }

      static inline VEC calculate(const VEC& p0,
                                   const VEC& p1,
                                   float t)
      {
        VEC result;
        result.x = (1 - t) * p0.x + t * p1.x;
        result.y = (1 - t) * p0.y + t * p1.y;
        return result;
      }

    };

    
    template<typename VEC>    
    struct QuadraticBezier : public Bezier<VEC>
    {
    private:
      QuadraticBezier(const VEC& p0,
                      const VEC& p1,
                      const VEC& p2,
                      boost::uint32_t steps = Bezier<VEC>::defaultSteps())
      {
        update(p0, p1, p2, steps);
      }
      
      QuadraticBezier(const VEC& corner,
                      const VEC& direction,
                      float radius,
                      boost::uint32_t steps = Bezier<VEC>::defaultSteps())
      {
        update(corner, direction, radius, steps);
      }

    public:
      static lost::shared_ptr<QuadraticBezier<VEC> > create(const VEC& p0,
                                       const VEC& p1,
                                       const VEC& p2,
                                       boost::uint32_t steps = Bezier<VEC>::defaultSteps())
      {
        return lost::shared_ptr<QuadraticBezier<VEC> >(new QuadraticBezier(p0, p1, p2, steps));
      }
      
      static lost::shared_ptr<QuadraticBezier<VEC> > create(const VEC& corner,
                                       const VEC& direction,
                                       float radius,
                                       boost::uint32_t steps = Bezier<VEC>::defaultSteps())
      {
        return lost::shared_ptr<QuadraticBezier<VEC> >(new QuadraticBezier(corner, direction, radius, steps));
      }
      
      static VEC* create(VEC* array,
                          boost::uint32_t offset,
                          const VEC& p0,
                          const VEC& p1,
                          const VEC& p2,
                          boost::uint32_t steps = Bezier<VEC>::defaultSteps())
      {
        boost::uint32_t idx = offset;
        float stepSize = 1.0 / steps;
        float t = 0.0;
        while (t <= 1.0)
        {
          array[idx++] = calculate(p0, p1, p2, t);
          t += stepSize;
        }
        return array;
      }
      
      static VEC* create(VEC* array,
                          boost::uint32_t offset,
                          const VEC& corner,
                          const VEC& direction,
                          float radius,
                          boost::uint32_t steps = Bezier<VEC>::defaultSteps())
      {
        VEC p1 = corner;
        float length = sin(45.0)*radius;
        VEC p0;
        VEC p2;
        if ((direction.x < 0 && direction.y > 0) || (direction.x > 0 && direction.y < 0))
        {
          p0 = VEC(p1.x+direction.x*length, p1.y);
          p2 = VEC(p1.x, p1.y+direction.y*length);
        }
        else
        {
          p0 = VEC(p1.x, p1.y+direction.y*length);
          p2 = VEC(p1.x+direction.x*length, p1.y);
        }
        return create(array, offset, p0, p1, p2, steps);
      }

      void update(const VEC& corner,
                  const VEC& direction,
                  float radius,
                  boost::uint32_t steps = Bezier<VEC>::defaultSteps())
      {
        VEC p1 = corner;
        float length = sin(45.0)*radius;
        VEC p0(p1.x, p1.y+direction.y*length);
        VEC p2(p1.x+direction.x*length, p1.y);
        update(p0, p1, p2, steps);        
      }
      
      void update(const VEC& p0,
                  const VEC& p1,
                  const VEC& p2,
                  boost::uint32_t steps = Bezier<VEC>::defaultSteps())
      {
        Bezier<VEC>::points.clear();
        float stepSize = 1.0 / steps;
        float t = 0.0;
        while (t <= 1.0)
        {
          Bezier<VEC>::points.push_back(calculate(p0, p1, p2, t));
          t += stepSize;
        }
      }
      
      static inline VEC calculate(const VEC& p0,
                                   const VEC& p1,
                                   const VEC& p2,
                                   float t)
      {
        VEC result;
        result.x = ((1 - t) * (1 - t)) * p0.x + (2*t) * (1-t) * p1.x + (t*t) * p2.x;
        result.y = ((1 - t) * (1 - t)) * p0.y + (2*t) * (1-t) * p1.y + (t*t) * p2.y;
        return result;
      }

    };

    
    template<typename VEC>    
    struct CubicBezier : public Bezier<VEC>
    {
    private:
      CubicBezier(const VEC& p0,
                  const VEC& p1,
                  const VEC& p2,
                  const VEC& p3,
                  boost::uint32_t steps = Bezier<VEC>::defaultSteps())
      {
        update(p0, p1, p2, p3, steps);
      }
      
    public:
      static lost::shared_ptr<CubicBezier<VEC> > create(const VEC& p0,
                                   const VEC& p1,
                                   const VEC& p2,
                                   const VEC& p3,
                                   boost::uint32_t steps = Bezier<VEC>::defaultSteps())
      {
        return lost::shared_ptr<CubicBezier<VEC> >(new CubicBezier(p0, p1, p2, p3, steps));
      }
    
      static VEC* create(VEC* array,
                          boost::uint32_t offset,
                          const VEC& p0,
                          const VEC& p1,
                          const VEC& p2,
                          const VEC& p3,
                          boost::uint32_t steps = Bezier<VEC>::defaultSteps())
      {
        boost::uint32_t idx = offset;
        float stepSize = 1.0 / steps;
        float t = 0.0;
        while (t <= 1.0)
        {
          array[idx++] = calculate(p0, p1, p2, p3, t);
          t += stepSize;
        }
        return array;
      }
      
      void update(const VEC& p0,
                  const VEC& p1,
                  const VEC& p2,
                  const VEC& p3,
                  boost::uint32_t steps = Bezier<VEC>::defaultSteps())
      {
        Bezier<VEC>::points.clear();
        float stepSize = 1.0 / steps;
        float t = 0.0;
        while (t <= 1.0)
        {
          Bezier<VEC>::points.push_back(calculate(p0, p1, p2, p3, t));
          t += stepSize;
        }
      }

      static inline VEC calculate(const VEC& p0,
                                   const VEC& p1,
                                   const VEC& p2,
                                   const VEC& p3,
                                   float t)
      {
        VEC result;
        result.x = (-p0.x + 3*p1.x - 3*p2.x + p3.x) * (t*t*t) + (3*p0.x - 6*p1.x + 3*p2.x) * (t*t) + (-3*p0.x + 3*p1.x) * t + p0.x;
        result.y = (-p0.y + 3*p1.y - 3*p2.y + p3.y) * (t*t*t) + (3*p0.y - 6*p1.y + 3*p2.y) * (t*t) + (-3*p0.y + 3*p1.y) * t + p0.y;
        return result;
      }
    };

    typedef LinearBezier<Vec2> LinearBezier2D;
    typedef LinearBezier<Vec3> LinearBezier3D;
    typedef QuadraticBezier<Vec2> QuadraticBezier2D;
    typedef QuadraticBezier<Vec3> QuadraticBezier3D;
    typedef CubicBezier<Vec2> CubicBezier2D;
    typedef CubicBezier<Vec3> CubicBezier3D;

    typedef lost::shared_ptr<LinearBezier2D> LinearBezier2DPtr;
    typedef lost::shared_ptr<LinearBezier3D> LinearBezier3DPtr;
    typedef lost::shared_ptr<QuadraticBezier2D> QuadraticBezier2DPtr;
    typedef lost::shared_ptr<QuadraticBezier3D> QuadraticBezier3DPtr;
    typedef lost::shared_ptr<CubicBezier2D> CubicBezier2DPtr;
    typedef lost::shared_ptr<CubicBezier3D> CubicBezier3DPtr;
    
  }
}

#endif

/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef LOST_MATH_BEZIER_H
#define LOST_MATH_BEZIER_H

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
      vector<VEC> points;

      static u32 defaultSteps()
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
                   u32 steps = Bezier<VEC>::defaultSteps())
      {
        update(p0, p1, steps);
      }

    public:
      static lost::shared_ptr<LinearBezier<VEC> > create(const VEC& p0,
                                    const VEC& p1,
                                    u32 steps = Bezier<VEC>::defaultSteps())
      {
        return lost::shared_ptr<LinearBezier<VEC> >(new LinearBezier<VEC>(p0, p1, steps));
      }

      static VEC* create(VEC* array,
                         u32 offset,
                         const VEC& p0,
                         const VEC& p1,
                         u32 steps = Bezier<VEC>::defaultSteps())
      {
        u32 idx = offset;
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
                  u32 steps = Bezier<VEC>::defaultSteps())
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
        result = (p0*(1.0f - t)) + (p1*t);
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
                      u32 steps = Bezier<VEC>::defaultSteps())
      {
        update(p0, p1, p2, steps);
      }
      
    public:
      static lost::shared_ptr<QuadraticBezier<VEC> > create(const VEC& p0,
                                       const VEC& p1,
                                       const VEC& p2,
                                       u32 steps = Bezier<VEC>::defaultSteps())
      {
        return lost::shared_ptr<QuadraticBezier<VEC> >(new QuadraticBezier(p0, p1, p2, steps));
      }
      
      static VEC* create(VEC* array,
                          u32 offset,
                          const VEC& p0,
                          const VEC& p1,
                          const VEC& p2,
                          u32 steps = Bezier<VEC>::defaultSteps())
      {
        u32 idx = offset;
        float stepSize = 1.0 / steps;
        float t = 0.0;
        while (t <= 1.0)
        {
          array[idx++] = calculate(p0, p1, p2, t);
          t += stepSize;
        }
        return array;
      }
      
      void update(const VEC& p0,
                  const VEC& p1,
                  const VEC& p2,
                  u32 steps = Bezier<VEC>::defaultSteps())
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
        result = (p0*((1.0f-t)*(1.0f-t))) + (p1*((2.0f*t)*(1.0f-t))) + (p2*(t*t));
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
                  u32 steps = Bezier<VEC>::defaultSteps())
      {
        update(p0, p1, p2, p3, steps);
      }
      
    public:
      static lost::shared_ptr<CubicBezier<VEC> > create(const VEC& p0,
                                   const VEC& p1,
                                   const VEC& p2,
                                   const VEC& p3,
                                   u32 steps = Bezier<VEC>::defaultSteps())
      {
        return lost::shared_ptr<CubicBezier<VEC> >(new CubicBezier(p0, p1, p2, p3, steps));
      }
    
      static VEC* create(VEC* array,
                          u32 offset,
                          const VEC& p0,
                          const VEC& p1,
                          const VEC& p2,
                          const VEC& p3,
                          u32 steps = Bezier<VEC>::defaultSteps())
      {
        u32 idx = offset;
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
                  u32 steps = Bezier<VEC>::defaultSteps())
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
        result = ((-1.0f*p0) + 3*p1 - 3*p2 + p3) * (t*t*t) + (3*p0 - 6*p1 + 3*p2) * (t*t) + (-3*p0 + 3*p1) * t + p0;
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

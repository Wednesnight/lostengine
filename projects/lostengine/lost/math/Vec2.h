#ifndef LOST_MATH_VEC2_H
#define LOST_MATH_VEC2_H


namespace lost
{
  namespace math
  {

    struct Vec2
    {
        union
        {
          struct {float x,y;};
          struct {float width,height;};
          struct {float min,max;};
        };

        Vec2(float a, float b);
        Vec2();
        void zero();
    };

    float len(const Vec2& inOp);
    float squlen(const Vec2& inOp);
    Vec2& normalise(Vec2& ioOp);
    Vec2 perpendicular(Vec2& v);
    Vec2 operator*(const Vec2& inOp1,  float v);
    Vec2 operator*(float v, const Vec2& inOp1);
    float operator*(const Vec2& inOp1, const Vec2& inOp2);
    Vec2 operator+(const Vec2& inOp1, const Vec2& inOp2);
    void operator +=(Vec2& inOp1, const Vec2& inOp2);
    Vec2 operator-(const Vec2& inOp1, const Vec2& inOp2);
    bool operator==(const Vec2& inOp1, const Vec2& inOp2);
    float angle(const Vec2& inOp1, const Vec2& inOp2);
    
    // compares two vectors, taking epsilon into account for rounding errors
    bool compare(const Vec2& l, const Vec2& r, float epsilon);
  }
}

#endif

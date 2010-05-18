#ifndef LOST_MATH_VEC3_H
#define LOST_MATH_VEC3_H


namespace lost
{
namespace math
{

struct Vec3
{
    union
    {
      struct {float x,y,z;};
      struct {float r,g,b;};
      struct {float v,vt,vn;};
    };



    Vec3();
    Vec3(float inx, float iny, float inz);
    void zero();
};

float len(const Vec3& inOp);
float squlen(const Vec3& inOp);
Vec3& normalise(Vec3& ioOp);
Vec3 operator*(const Vec3& inOp1,  float v);
float operator*(const Vec3& inOp1, const Vec3& inOp2);
Vec3 cross(const Vec3& inOp1, const Vec3& inOp2);
float angle(const Vec3& inOp1, const Vec3& inOp2);
Vec3 operator+(const Vec3& inOp1, const Vec3& inOp2);
void operator +=(Vec3& inOp1, const Vec3& inOp2);
Vec3 operator-(const Vec3& inOp1, const Vec3& inOp2);
void operator -=(Vec3& inOp1, const Vec3& inOp2);
bool operator ==(const Vec3& inOp1, const Vec3& inOp2);
// compares two vectors, taking epsilong into account for rounding errors
bool compare(const Vec3& l, const Vec3& r, float epsilon);

}
}

#endif

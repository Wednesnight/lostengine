#include "UnitTest++.h"

#include "lost/math/lmath.h"
#include "lost/math/Quat.h"
#include "lost/math/Matrix.h"

using namespace lost::math;

SUITE(QuatTest)
{
  TEST(compile)
  {  
    Quat q;
    CHECK(&q);
  }

  TEST(normalize)
  {
    Quat q(10.0f, 10.0f, 10.0f, 10.0f);
    q = q.normalize();
    CHECK(q.x <=  1.0f && q.y <=  1.0f && q.z <=  1.0f && q.w <=  1.0f &&
          q.x >= -1.0f && q.y >= -1.0f && q.z >= -1.0f && q.w >= -1.0f);
  }

  TEST(rotation)
  {
    float epsilon = 0.00001f;

    Quat       qRotate(1.0f,0.0f,0.0f,45.0f);
    Matrix     qRotateMatrix = qRotate.matrix();
    MatrixRotX mRotate(45.0f);
    for(unsigned long i=0; i<Matrix::numvals; ++i)
    {
      CHECK(compare(qRotateMatrix.m[i], mRotate.m[i], epsilon));
    }
    
    Vec3 v = Vec3(10,10,10);
    Vec3 v1 = qRotate.matrix()*v;
    Vec3 v2 = mRotate*v;
    CHECK(compare(v1.x, v2.x, epsilon) && compare(v1.y, v2.y, epsilon) && compare(v1.z, v2.z, epsilon));

    v1 = qRotate.rotate(v); std::cout << v1 << std::endl;
    v2 = mRotate*v; std::cout << v2 << std::endl;
    CHECK(compare(v1.x, v2.x, epsilon) && compare(v1.y, v2.y, epsilon) && compare(v1.z, v2.z, epsilon));
  }
/*  
  TEST(comparison)
  {
    Vec4 v1, v2;
    CHECK(v1 == v2);
    
    v1.x = 1;
    v1.y = 2;
    v1.z = 3;
    v1.w = 4;
    
    v2.x = 1;
    v2.y = 2;
    v2.z = 3;
    v2.w = 4;
    CHECK(v1 == v2);
    
    v2.w = 1;
    v2.z = 2;
    v2.y = 3;
    v2.x = 4;
    CHECK(!(v1 == v2));
  }
  
  TEST(multiply)
  {
    Vec4 v1(2, 1, 1, 1), v2(1, 1, 1,1);
    
    float v = v1*v2;
    CHECK(v == 5);
  }
*/
}

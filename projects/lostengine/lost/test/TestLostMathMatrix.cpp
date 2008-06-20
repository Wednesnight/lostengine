#include "lost/math/Matrix.h"
#include "UnitTest++.h"
#include "lost/common/Logger.h"
using namespace lost::math;
SUITE(MatrixTest)
{
TEST(compile)
{  
  Matrix matrix;
  CHECK(&matrix);
}

TEST(matrix_matrix_multiply)
{
  Matrix m1, m2, m3;
  m1.initIdentity();
  m2.initIdentity();
  m3 = m1 * m2;
  CHECK(m1 == m2);
  CHECK(m3 == m1);
}

TEST(matrix_vec_multiply)
{
  Matrix m;
  Vec4 v1(1, 1, 1, 1), v2;
  m.initIdentity();
  v2 = m * v1;
  CHECK(v2 == v1);
  m.initTranslation(Vec3(3, 2, 1));
  v2 = m * v1;
  CHECK(v2 == Vec4(4, 3, 2, 1));
}

TEST(test_scaling)
{
  Matrix scale;
  Vec4 src(1, 1, 1, 1);
  Vec4 res;
  scale.initScaling(Vec3(1, 2, 1));
  res = scale * src;
  CHECK(res == Vec4(1, 2, 1, 1));
}

TEST(matrix_combine_scale_translate)
{
  Matrix scale, translate, composite;
  scale.initScaling(Vec3(1, 2, 3));
  translate.initTranslation(Vec3(4, 5, 6));
  composite = translate * scale;
  Vec4 res, src(1, 1, 1, 1);
  res = composite *src; 
  CHECK(res == Vec4(5, 7, 9, 1));
}

TEST(rotatex)
{
  float epsilon = 1.0e-07;
  Matrix rotx;
  Vec4 dest, src(0,1,0,1);
  rotx.initRotateX(-90);
  dest = rotx*src;
  Vec4 correct(0, 0, 1, 1);
  CHECK(compare(dest, correct, epsilon));
  
  rotx.initRotateX(90);
  dest = rotx*src;
  correct = Vec4(0, 0, -1, 1);
  CHECK(compare(dest, correct, epsilon));
}

TEST(rotatey)
{
  float epsilon = 1.0e-07;
  Matrix roty;
  Vec4 dest, src(1, 0, 0, 1);

  roty.initRotateY(90);
  dest = roty * src;
  Vec4 correct(0, 0, 1, 1);
  CHECK(compare(dest, correct, epsilon));
  
  roty.initRotateY(-90);
  dest = roty * src;
  correct = Vec4(0, 0, -1, 1);
  CHECK(compare(dest, correct, epsilon));
}

TEST(rotatez)
{
  float epsilon = 1.0e-07;
  Matrix rotz;
  Vec4 dest, src(0, 1, 0, 1);
  
  rotz.initRotateZ(90);
  dest = rotz * src;
  Vec4 correct(1, 0, 0, 1);
  CHECK(compare(dest, correct, epsilon));

  rotz.initRotateZ(-90);
  dest = rotz * src;
  correct = Vec4(-1, 0, 0, 1);
  CHECK(compare(dest, correct, epsilon));
}

}

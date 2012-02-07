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

#include "lost/math/Vec4.h"
#include "UnitTest++.h"

using namespace lost::math;
SUITE(Vec4Test)
{
TEST(compile)
{  
  Vec4 vec;
  CHECK(&vec);
}

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

}

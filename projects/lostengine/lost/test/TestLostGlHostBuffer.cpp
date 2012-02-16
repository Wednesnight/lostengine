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

#include "lost/gl/HostBuffer.h"
#include "UnitTest++.h"
/*
using namespace lost::gl;
using namespace lost::math;

TEST(hostbuffer_strideoffset_vtx)
{
  BufferLayout layout;
  layout.add(ET_vec2_f32, UT_position, 0);
  layout.add(ET_vec2_f32, UT_texcoord0, 0);
  HostBuffer buffer(layout);
  buffer.reset(10);
  
  CHECK(buffer.layout.stride(UT_position) == 16);
  CHECK(buffer.layout.stride(UT_texcoord0) == 16);
  CHECK(buffer.layout.offset(UT_position) == 0);
  CHECK(buffer.layout.offset(UT_texcoord0) == 8);
}

TEST(hostbuffer_getsetVec2)
{
  BufferLayout layout;
  layout.add(ET_vec2_f32, UT_position, 0);
  layout.add(ET_vec2_f32, UT_texcoord0, 0);
  HostBuffer buffer(layout);
  buffer.reset(10);

  buffer.set(0,UT_position, Vec2(0,1));
  buffer.set(0,UT_texcoord0, Vec2(1,2));

  buffer.set(7,UT_position, Vec2(0,7));
  buffer.set(7,UT_texcoord0, Vec2(7,2));
  
  CHECK(buffer.getAsVec2(0,UT_position) == Vec2(0,1));
  CHECK(buffer.getAsVec2(0,UT_texcoord0) == Vec2(1,2));
  CHECK(buffer.getAsVec2(7,UT_position) == Vec2(0,7));
  CHECK(buffer.getAsVec2(7,UT_texcoord0) == Vec2(7,2));
}

TEST(hostbuffer_strideoffset_idx)
{
  BufferLayout layout;
  layout.add(ET_u32, UT_index, 0);
  HostBuffer buffer(layout);
  buffer.reset(10);
  
  CHECK(buffer.layout.stride(UT_index) == 4);
  CHECK(buffer.layout.offset(UT_index) == 0);
}

TEST(hostbuffer_getsetU32)
{
  BufferLayout layout;
  layout.add(ET_u32, UT_index, 0);
  HostBuffer buffer(layout);
  buffer.reset(10);

  buffer.set(0, UT_index, (uint32_t)13);
  buffer.set(7, UT_index, (uint32_t)8);
  
  CHECK(buffer.getAsU32(0, UT_index) == 13);
  CHECK(buffer.getAsU32(7, UT_index) == 8);
}
*/

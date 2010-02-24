#include "lost/gl/HostBuffer.h"
#include "UnitTest++.h"

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
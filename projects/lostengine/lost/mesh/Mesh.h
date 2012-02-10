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

#ifndef LOST_MESH_MESH
#define LOST_MESH_MESH

#include "lost/mesh/Material.h"
#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"
#include "lost/math/Vec4.h"
#include "lost/math/Matrix.h"
#include "lost/gl/BufferLayout.h"
#include "lost/mesh/forward.h"
#include "lost/gl/forward.h"

namespace lost
{

namespace mesh
{

struct Mesh
{
  Mesh();
  Mesh(const gl::BufferLayout& vertexLayout, gl::ElementType indexType);
  // completely initializes the mesh object with default values. This is usually called from a constructor
  void init(const gl::BufferLayout& vertexLayout, gl::ElementType indexType);
  virtual ~Mesh();
  
  static MeshPtr create() { return MeshPtr(new Mesh); }
  static MeshPtr create(const gl::BufferLayout& vertexLayout, gl::ElementType indexType) { return MeshPtr(new Mesh(vertexLayout, indexType)); }
  
  // resets the buffer types, throwing away all previously stored/allocated data. you need to call resetSize after this one.
  void resetBuffers(const gl::BufferLayout& vertexLayout, gl::ElementType indexType);

  // resets the size of vertex and index buffers
  void resetSize(uint32_t numVertices, uint32_t numIndices);

  // helper functions that could easily be replaced by direct access of buffers but were needed for migration
  // FIXME: these are here for legacy reasons, should really be replaced with the versions below
  void setIndex(uint32_t idx, uint32_t val);
  void setVertex(uint32_t idx, const math::Vec2& val);
  void setTexCoord(uint32_t idx, const math::Vec2& val);
  math::Vec2 getVertex(uint32_t idx);

  gl::HybridBufferPtr bufferFromUsageType(gl::UsageType ut);

  void set(uint32_t idx, gl::UsageType ut, uint8_t val);
  void set(uint32_t idx, gl::UsageType ut, uint16_t val);
  void set(uint32_t idx, gl::UsageType ut, uint32_t val);
  void set(uint32_t idx, gl::UsageType ut, float val);
  void set(uint32_t idx, gl::UsageType ut, const math::Vec2& val);
  void set(uint32_t idx, gl::UsageType ut, const math::Vec3& val);
  void set(uint32_t idx, gl::UsageType ut, const math::Vec4& val);
  void set(uint32_t idx, gl::UsageType ut, const common::Color& val);
  
  math::Vec2 getAsVec2(uint32_t idx, gl::UsageType ut);
  math::Vec3 getAsVec3(uint32_t idx, gl::UsageType ut);
  uint32_t   getAsU32(uint32_t idx, gl::UsageType ut);  
  uint16_t   getAsU16(uint32_t idx, gl::UsageType ut);  

  MaterialPtr material;
  math::Matrix transform;
  gl::HybridVertexBufferPtr vertexBuffer;
  gl::HybridIndexBufferPtr indexBuffer;
  
  virtual MeshPtr clone();
  uint32_t numVertices(); // returns the current size of the vertexbuffer
};

}
}

#endif

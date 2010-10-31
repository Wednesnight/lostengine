module("lost.common", package.seeall)

require("lost.common.Class")

using "lost.math.Vec2"
using "lost.math.Vec3"
using "lost.math.Rect"
using "lost.common.Color"
using "lost.bitmap.Bitmap"
using "lost.math.MatrixTranslation"
using "lost.math.MatrixRotZ"

lost.common.Class "lost.common.MeshFactory" {}

function MeshFactory:constructor(shaderFactory)
  self.shaderFactory = shaderFactory
end

function MeshFactory:createQuad(size)
  local layout = lost.gl.BufferLayout()
  layout:add(gl.ET_vec2_f32, gl.UT_position, 0)
  layout:add(gl.ET_vec2_f32, gl.UT_texcoord0, 0)
  local result = lost.mesh.Mesh.create(layout, gl.ET_u16)
  result.indexBuffer.drawMode = gl.GL_TRIANGLES
  local numVerts = 4
  local numTris = 2
  local numIndicesPerTri = 3
  local numIndices = numTris * numIndicesPerTri
  result:resetSize(numVerts, numIndices)

  result:set(0,gl.UT_position, Vec2(0,0))
  result:set(1,gl.UT_position, Vec2(size.x,0))
  result:set(2,gl.UT_position, Vec2(size.x,size.y))
  result:set(3,gl.UT_position, Vec2(0,size.y))

  result:set(0,gl.UT_texcoord0, Vec2(0,0))
  result:set(1,gl.UT_texcoord0, Vec2(1,0))
  result:set(2,gl.UT_texcoord0, Vec2(1,1))
  result:set(3,gl.UT_texcoord0, Vec2(0,1))

  result:setU16(0,gl.UT_index, 0)
  result:setU16(1,gl.UT_index, 1)
  result:setU16(2,gl.UT_index, 2)
  result:setU16(3,gl.UT_index, 2)
  result:setU16(4,gl.UT_index, 3)
  result:setU16(5,gl.UT_index, 0)
  
  result.material.uniforms = lost.gl.UniformBlock.create()
  result.material:blendPremultiplied()
  
  return result
end

function MeshFactory:createDisc(col, pos, radius)
  local size = Vec2(radius*2, radius*2)
  local result = self:createQuad(size)
  result.material.color = col
  result.material.shader = self.shaderFactory:disc()
  result.material.uniforms:set("size", size)
  result.material.uniforms:set("center", Vec2(radius-1, radius-1))
  result.material.uniforms:setFloat("radius", radius)
  result.transform = MatrixTranslation(Vec3(pos.x,pos.y,0))  
  return result
end

function MeshFactory:createRing(col, pos, radius, width)
  local size = Vec2(radius*2, radius*2)
  local result = self:createQuad(size)
  result.material.color = col
  result.material.shader = self.shaderFactory:ring()
  result.material.uniforms:set("size", size)
  result.material.uniforms:set("center", Vec2(radius-1, radius-1))
  result.material.uniforms:setFloat("radius", radius)
  result.material.uniforms:setFloat("width", width)
  result.transform = MatrixTranslation(Vec3(pos.x,pos.y,0))  
  return result
end

function MeshFactory:createRoundedRect(col, rect, radius)
  local size = Vec2(rect.width, rect.height)
  local result = self:createQuad(size)

  result.material.color = col
  result.material.shader = self.shaderFactory:roundedRect(true, {}, {})
  result.material.uniforms:set("size", size)
  result.material.uniforms:setFloat("radius", radius)
  result.transform = MatrixTranslation(Vec3(rect.x,rect.y,0))
  
  return result
end

function MeshFactory:createRoundedRectFrame(col, rect, radius, width)
  local size = Vec2(rect.width, rect.height)
  local result = self:createQuad(size)

  result.material.color = col
  result.material.shader = self.shaderFactory:roundedRect(false, {}, {}) 
  result.material.uniforms:set("size", size)
  result.material.uniforms:setFloat("radius", radius)
  result.material.uniforms:setFloat("width", width)
  result.transform = MatrixTranslation(Vec3(rect.x,rect.y,0))
  
  return result
end

function MeshFactory:createBox(col, rect)
  local size = Vec2(rect.width, rect.height)
  local result = self:createQuad(size)
  result.material.color = col
  result.material.shader = self.shaderFactory:box()
  result.material.uniforms:set("size", size)
  result.transform = MatrixTranslation(Vec3(rect.x,rect.y,0))*MatrixRotZ(-87)
  result.material:blendPremultiplied()
  return result
end

function MeshFactory:iqcreateRoundedRect(col, rect, radius)
  local result = self:createRoundedRect(col, rect, radius)
  result.material.shader = self.shaderFactory:iqrr()
  return result
end

function MeshFactory:iqcreateRoundedRectFrame(col, rect, radius, width)
  local result = self:createRoundedRectFrame(col, rect, radius, width)
  result.material.shader = self.shaderFactory:iqrrf()
  return result
end

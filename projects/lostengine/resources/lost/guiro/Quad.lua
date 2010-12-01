module("lost.guiro", package.seeall)

require("lost.common.Class")

lost.common.Class "lost.guiro.Quad" {}

-- basic geometry for guiro layers.
-- encapsulates rules for mesh creation, a mesh instance, and provides helper functions for 
-- geometry updates.
function Quad:constructor(args)
  local t = args or {}
  
  local rect = t.rect or lost.math.Rect()
  local layout = lost.gl.BufferLayout()
  layout:add(gl.ET_vec2_f32, gl.UT_position, 0)
  layout:add(gl.ET_vec2_f32, gl.UT_texcoord0, 0)
  self.mesh = lost.mesh.Mesh.create(layout, gl.ET_u16)
  self.mesh.indexBuffer.drawMode = gl.GL_TRIANGLES
  local numVerts = 4
  local numTris = 2
  local numIndicesPerTri = 3
  local numIndices = numTris * numIndicesPerTri
  self.mesh:resetSize(numVerts, numIndices)

  self.mesh:set(0,gl.UT_position, Vec2(rect.x,rect.y))
  self.mesh:set(1,gl.UT_position, Vec2(rect.x+rect.width,rect.y))
  self.mesh:set(2,gl.UT_position, Vec2(rect.x+rect.width,rect.y+rect.height))
  self.mesh:set(3,gl.UT_position, Vec2(rect.x,rect.y+rect.height))

  self.mesh:set(0,gl.UT_texcoord0, Vec2(0,0))
  self.mesh:set(1,gl.UT_texcoord0, Vec2(1,0))
  self.mesh:set(2,gl.UT_texcoord0, Vec2(1,1))
  self.mesh:set(3,gl.UT_texcoord0, Vec2(0,1))

  self.mesh:setU16(0,gl.UT_index, 0)
  self.mesh:setU16(1,gl.UT_index, 1)
  self.mesh:setU16(2,gl.UT_index, 2)
  self.mesh:setU16(3,gl.UT_index, 2)
  self.mesh:setU16(4,gl.UT_index, 3)
  self.mesh:setU16(5,gl.UT_index, 0)

  self.mesh.material.uniforms = lost.gl.UniformBlock.create()
  self.mesh.material:blendPremultiplied()
end

function Quad:update(rect)
  self.mesh:set(0,gl.UT_position, Vec2(rect.x,rect.y))
  self.mesh:set(1,gl.UT_position, Vec2(rect.x+rect.width,rect.y))
  self.mesh:set(2,gl.UT_position, Vec2(rect.x+rect.width,rect.y+rect.height))
  self.mesh:set(3,gl.UT_position, Vec2(rect.x,rect.y+rect.height))
end
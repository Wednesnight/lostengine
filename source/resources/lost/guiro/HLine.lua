module("lost.guiro", package.seeall)

require("lost.common.Class")

lost.common.Class "lost.guiro.HLine" {}

local Vec2 = lost.math.Vec2

-- horizontal line Mesh
function HLine:constructor(args)
  local t = args or {}
  
  local rect = t.rect or lost.math.Rect()
  local layout = lost.gl.BufferLayout()
  layout:add(gl.ET_vec2_f32, gl.UT_position)
  layout:add(gl.ET_vec2_f32, gl.UT_texcoord0)
  self.mesh = lost.mesh.Mesh.create(layout, gl.ET_u16)
  self.mesh.indexBuffer.drawMode = gl.GL_LINES
  local numVerts = 2
  local numIndices = 2
  self.mesh:resetSize(numVerts, numIndices)

  self.mesh:set(0,gl.UT_position, Vec2(rect.x,rect.y))
  self.mesh:set(1,gl.UT_position, Vec2(rect.x+rect.width,rect.y))

  self.mesh:set(0,gl.UT_texcoord0, Vec2(0,0))
  self.mesh:set(1,gl.UT_texcoord0, Vec2(1,0))

  self.mesh:setU16(0,gl.UT_index, 0)
  self.mesh:setU16(1,gl.UT_index, 1)

  self.mesh.material.uniforms = lost.gl.UniformBlock.create()
  self.mesh.material:blendPremultiplied()
end

function HLine:update(rect)
  self.mesh:set(0,gl.UT_position, Vec2(rect.x,rect.y))
  self.mesh:set(1,gl.UT_position, Vec2(rect.x+rect.width,rect.y))
end
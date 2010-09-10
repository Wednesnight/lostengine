module("aqua", package.seeall)

require("lost.common.Class")
require("lost.common.Shaders")

lost.common.Class "Cloud" {}

local Color = lost.common.Color

function Cloud:constructor(color, pos, vel)
  aqua.Entity.constructor(self)
  self.name = "Cloud"
  self.bitmap = lost.bitmap.Bitmap.create(tasklet.loader:load("cloud.png"))
  self.cloudTex = lost.gl.Texture.create(self.bitmap,lost.gl.Texture.Params())
  self.quad = lost.mesh.Quad.create(self.cloudTex, true)
  self.quad.material.shader = lost.common.Shaders.textureShader()
  self.quad.material.blend = true
  self.quad.material.color = color
  self.drawNode = lost.rg.Draw.create(self.quad)
  self.renderNode:add(self.drawNode)
  self.velocity = vel
  self.pos = pos
end

function Cloud:update(dt, world)
  local x = math.fmod(self.pos.x+self.velocity*dt,world.screenSize.x) 
  self.pos = Vec2(x, self.pos.y)
  self.quad.transform = lost.math.MatrixTranslation(lost.math.Vec3(self.pos.x,self.pos.y,0))
end


module("aqua", package.seeall)

require("lost.common.Class")
require("lost.common.Shaders")

lost.common.Class "Cloud" {}

local Color = lost.common.Color

function Cloud:constructor()
  aqua.Entity.constructor(self)
  self.name = "Cloud"
  self.bitmap = lost.bitmap.Bitmap.create(tasklet.loader:load("cloud.png"))
  self.cloudTex = lost.gl.Texture.create(self.bitmap,lost.gl.Texture.Params())
  self.quad = lost.mesh.Quad.create(self.cloudTex, true)
  self.quad.material.shader = lost.common.Shaders.textureShader()
  self.quad.material.blend = true
--  self.quad.material.color = Color(1,.7,.7)
  self.drawNode = lost.rg.Draw.create(self.quad)
  self.renderNode:add(self.drawNode)
end

function Cloud:update(dt, world)
  
end


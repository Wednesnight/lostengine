module("aqua", package.seeall)

require("lost.common.Class")

lost.common.Class "Ground" {}

function Ground:constructor(color, pos)
  aqua.Entity.constructor(self)
  self.name = "Ground"
  self.bitmap = lost.bitmap.Bitmap.create(tasklet.loader:load("BG_ground2.png"))
  self.cloudTex = lost.gl.Texture.create(self.bitmap,lost.gl.Texture.Params())
  self.quad = lost.mesh.Quad.create(self.cloudTex, true)
  self.quad.material.shader = lost.common.Shaders.textureShader()
  self.quad.material.blend = true
  self.quad.material.color = color
  self.drawNode = lost.rg.Draw.create(self.quad)
  self.renderNode:add(self.drawNode)
  self.pos = pos
end

function Ground:update(dt, world)
  self.quad.transform = lost.math.MatrixTranslation(lost.math.Vec3(self.pos.x,self.pos.y,0))  
end
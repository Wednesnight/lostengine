module("aqua", package.seeall)

require("lost.common.Class")
require("Physics")

lost.common.Class "Ground" "aqua.Entity" {}

function Ground:constructor(color, pos, restitution)
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
  self.restitution = restitution or 0.0
end

function Ground:init(world)
  self.bodyDef = box2d.b2BodyDef()
  self.bodyDef.position:Set(Physics.screenToWorld(self.pos.x, self.pos.y))
  self.body = world.physics:CreateBody(self.bodyDef)
  self.shapeDef = box2d.b2PolygonDef()
  local x, y = Physics.screenToWorld(14, 14)
  self.shapeDef:SetAsBox(x, y, Physics.screenToWorldVec2(box2d.b2Vec2(14, 14)), 0)
  self.shapeDef.friction = 0.6
  self.shapeDef.restitution = self.restitution
  self.shape = self.body:CreateShape(self.shapeDef)
  return true
end

function Ground:update(dt, world)
  self.quad.transform = lost.math.MatrixTranslation(lost.math.Vec3(self.pos.x,self.pos.y,0))  
end

module("aqua", package.seeall)

require("lost.common.Class")

lost.common.Class "Player" "aqua.Entity" {}

function Player:constructor(color, pos)
  aqua.Entity.constructor(self)
  self.name = "Player"
  self.bitmap = lost.bitmap.Bitmap.create(tasklet.loader:load("aqua.png"))
  self.playerTex = lost.gl.Texture.create(self.bitmap,lost.gl.Texture.Params())
  self.quad = lost.mesh.Quad.create(self.playerTex, true)
  self.quad.material.shader = lost.common.Shaders.textureShader()
  self.quad.material.blend = true
  self.quad.material.color = color
  self.drawNode = lost.rg.Draw.create(self.quad)
  self.renderNode:add(self.drawNode)
  self.pos = pos
  self.maxVelocity = lost.math.Vec2(15, 30)
end

function Player:init(world)
  self.bodyDef = box2d.b2BodyDef()
  self.bodyDef.position:Set(self.pos.x, self.pos.y)
  self.bodyDef.fixedRotation = true
  self.body = world.physics:CreateBody(self.bodyDef)
  self.shapeDef = box2d.b2PolygonDef()
  self.shapeDef:SetAsBox(22, 32, box2d.b2Vec2(22, 32), 0)
  self.shapeDef.density = 1
--  self.shapeDef.friction = 0.8
  self.shape = self.body:CreateShape(self.shapeDef)
  self.body:SetMassFromShapes()
  return true
end

function Player:update(dt, world)
  local newPos = self.body:GetPosition()
  self.pos.x = newPos.x
  self.pos.y = newPos.y
  self.quad.transform = lost.math.MatrixTranslation(lost.math.Vec3(self.pos.x,self.pos.y,0))  
end

function Player:left()
  local velocity = -self.maxVelocity.x - self.body:GetLinearVelocity().x
  self.body:ApplyImpulse(box2d.b2Vec2(velocity * self.body:GetMass(), 0), self.body:GetWorldCenter())
end

function Player:right()
  local velocity = self.maxVelocity.x - self.body:GetLinearVelocity().x
  self.body:ApplyImpulse(box2d.b2Vec2(velocity * self.body:GetMass(), 0), self.body:GetWorldCenter())
end

function Player:jump()
  local velocity = self.maxVelocity.y - self.body:GetLinearVelocity().y
  self.body:ApplyImpulse(box2d.b2Vec2(0, velocity * self.body:GetMass()), self.body:GetWorldCenter())
end

function Player:duck()
end

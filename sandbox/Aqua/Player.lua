module("aqua", package.seeall)

require("lost.common.Class")
require("Physics")

local config = require("config")

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
  self.speed = {
    ground = config.player.groundSpeed or lost.math.Vec2(10, 16),
    air = config.player.airSpeed or lost.math.Vec2(5, 16)
  }
end

function Player:init(world)
  self.bodyDef = box2d.b2BodyDef()
  self.bodyDef.position:Set(Physics.screenToWorld(self.pos.x, self.pos.y))
  self.bodyDef.fixedRotation = true
  self.body = world.physics:CreateBody(self.bodyDef)
  self.shapeDef = box2d.b2PolygonDef()
  local x, y = Physics.screenToWorld(22, 32)
  self.shapeDef:SetAsBox(x, y, Physics.screenToWorldVec2(box2d.b2Vec2(22, 32)), 0)
  self.shapeDef.density = 1.0
  self.shapeDef.friction = 1.2
  self.shape = self.body:CreateShape(self.shapeDef)
  self.body:SetMassFromShapes()
  self.massData = box2d.b2MassData()
  self.massData.mass = config.player.mass or 80.0
  self.massData.center = self.body:GetLocalCenter()
  self.massData.I = 0.0
  self.body:SetMass(self.massData)
  return true
end

function Player:update(dt, world)
  local newPos = Physics.worldToScreenVec2(self.body:GetPosition())
  self.pos.x = newPos.x
  self.pos.y = newPos.y
  self.quad.transform = lost.math.MatrixTranslation(lost.math.Vec3(self.pos.x,self.pos.y,0))  
end

function Player:setVelocity(x, y)
  local currentVelocity = self.body:GetLinearVelocity()
  if x ~= nil then
    currentVelocity.x = x
  end
  if y ~= nil then
    currentVelocity.y = y
  end
  self.body:SetLinearVelocity(currentVelocity)
end

--[[
    Apply velocity using corrective impulses. See: http://www.box2d.org/forum/viewtopic.php?p=25742#p25742
  ]]
function Player:applyVelocity(x, y)
  local velocity = self.body:GetLinearVelocity()

  if x == nil then
    x = 0
  elseif x ~= 0 then
    x = self.body:GetMass() * (x - velocity.x)
  end

  if y == nil then
    y = 0
  elseif y ~= 0 then
    y = self.body:GetMass() * (y - velocity.y)
  end

  if x ~= 0 or y ~= 0 then
    self.body:ApplyImpulse(box2d.b2Vec2(x, y), self.body:GetWorldCenter())
  end
end

function Player:left()
  local speed = -self.speed.ground.x
  if self.body:GetLinearVelocity().y ~= 0 then
    speed = -self.speed.air.x
  end
  self:applyVelocity(speed, 0)
end

function Player:right()
  local speed = self.speed.ground.x
  if self.body:GetLinearVelocity().y ~= 0 then
    speed = self.speed.air.x
  end
  self:applyVelocity(speed, 0)
end

function Player:jump()
  if self.jumpCount == nil then
    self.jumpCount = 0
  end

  if self.jumpCount < 2 or self.body:GetLinearVelocity().y == 0 then
    if self.jumpCount < 2 then
      self.jumpCount = self.jumpCount + 1
      self:applyVelocity(0, self.speed.air.y)
    else
      self.jumpCount = 1
      self:applyVelocity(0, self.speed.ground.y)
    end
  end
end

function Player:duck()
end

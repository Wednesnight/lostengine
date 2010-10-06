module("aqua", package.seeall)

require("lost.common.Class")
require("Player")
require("lost.common.Shaders")
require("Cloud")
require("Ground")
require("Physics")

local config = require("config")
local Color = lost.common.Color
local Rect = lost.math.Rect

lost.common.Class "World" {}


function World:constructor()
  self.entities = {}

  tasklet.clearNode.active = false
  
  self.physicsFrequency = 1.0 / 60.0

  local aabb = box2d.b2AABB()
  aabb.lowerBound:Set(Physics.screenToWorld(-config.window.width, -config.window.height))
  aabb.upperBound:Set(Physics.screenToWorld(config.window.width*2, config.window.height*2))
  local gravity = box2d.b2Vec2(Physics.getGravity().x, Physics.getGravity().y)
  local doSleep = true
  self.physics = box2d.b2World(aabb, gravity, doSleep)

  -- level bounds
  -- bottom/left
  local bodyDef = box2d.b2BodyDef()
  bodyDef.position:Set(Physics.screenToWorld(0, -2))
  local body = self.physics:CreateBody(bodyDef)
  local shapeDef = box2d.b2PolygonDef()
  local x, y = Physics.screenToWorld(config.window.width/2, 1)
  shapeDef:SetAsBox(x, y, Physics.screenToWorldVec2(box2d.b2Vec2(config.window.width/2, 1)), 0)
  body:CreateShape(shapeDef)
  bodyDef = box2d.b2BodyDef()
  bodyDef.position:Set(Physics.screenToWorld(-2, 0))
  body = self.physics:CreateBody(bodyDef)
  shapeDef = box2d.b2PolygonDef()
  x, y = Physics.screenToWorld(1, config.window.height/2)
  shapeDef:SetAsBox(x, y, Physics.screenToWorldVec2(box2d.b2Vec2(1, config.window.height/2)), 0)
  body:CreateShape(shapeDef)
  -- top/right
  bodyDef = box2d.b2BodyDef()
  bodyDef.position:Set(Physics.screenToWorld(0, config.window.height))
  body = self.physics:CreateBody(bodyDef)
  shapeDef = box2d.b2PolygonDef()
  x, y = Physics.screenToWorld(config.window.width/2, 1)
  shapeDef:SetAsBox(x, y, Physics.screenToWorldVec2(box2d.b2Vec2(config.window.width/2, 1)), 0)
  body:CreateShape(shapeDef)
  bodyDef = box2d.b2BodyDef()
  bodyDef.position:Set(Physics.screenToWorld(config.window.width, 0))
  body = self.physics:CreateBody(bodyDef)
  shapeDef = box2d.b2PolygonDef()
  x, y = Physics.screenToWorld(1, config.window.height/2)
  shapeDef:SetAsBox(x, y, Physics.screenToWorldVec2(box2d.b2Vec2(1, config.window.height/2)), 0)
  body:CreateShape(shapeDef)

  self.renderNode = lost.rg.Node.create()
  self.renderNode:add(lost.rg.ClearColor.create(lost.common.Color(0,0,0)))
  self.renderNode:add(lost.rg.Clear.create(gl.GL_COLOR_BUFFER_BIT))
  
  self.camera = lost.camera.Camera2D.create(lost.math.Rect(0,0,config.window.width, config.window.height))
  self.camNode = lost.rg.Camera.create(self.camera)
  
  
  self.renderNode = dcl.rg:Node
  {
    dcl.rg:ClearColor
    {
      color = Color(92/255,174/255,210/255,0)
    },
    dcl.rg:Clear
    {
      mask = gl.GL_COLOR_BUFFER_BIT
    },
    dcl.rg:DepthTest{false},
    dcl.rg:Camera2D
    {
      viewport = Rect(0,0,config.window.width, config.window.height)
    },    
    dcl.rg:Draw
    {
      mesh = dcl.mesh:Quad
      {
        filename = "BG_ground2.png",
        flip = true,
        material = 
        {
          blend = true,
          color = Color(1,1,1),
          shader = lost.common.Shaders.textureShader()
        }
      }
    }    
  }
    
  tasklet.renderNode:add(self.renderNode)
  self.screenSize = Vec2(config.window.width, config.window.height)
  log.debug("!!!!!!!!!! screensize "..tostring(self.screenSize))

  self:addEntity(aqua.Cloud(Color(1,1,1),Vec2(50,400), 13))
  self:addEntity(aqua.Cloud(Color(1,.7,.7),Vec2(150,350), -8))
  self:addEntity(aqua.Cloud(Color(1,1,.5),Vec2(230,380), 12))
  self:addEntity(aqua.Cloud(Color(1,1,1),Vec2(75,280), -10))
  self:addEntity(aqua.Cloud(Color(.8,.8,.8),Vec2(48,265), 15))
  
  local w = 32
  for i = 0,19,1 do
--    log.debug(i*w)
    if i > 4 and i < 8 then
      self:addEntity(aqua.Ground(Color(1,0,0),Vec2(i*w,0), 1.4))
    else
      self:addEntity(aqua.Ground(Color(1,1,1),Vec2(i*w,0)))
    end
  end

  for i = 10,14,1 do
--    log.debug(i*w)
    self:addEntity(aqua.Ground(Color(1,1,1),Vec2(i*w,192)))
  end

  self.player = aqua.Player(Color(1,1,1), Vec2(0,28))
  self:addEntity(self.player)

end

function World:addEntity(ent)
--  log.debug("!!!!!! adding entity: "..ent.name)
  if ent:init(self) then
    table.insert(self.entities, ent)
    self.renderNode:add(ent.renderNode)
  end
end

function World:updateEntities(dt)

  self.physicsFrequency = self.physicsFrequency + dt
  if self.physicsFrequency >= 1.0 / 60.0 then
    self.physics:Step(1.0 / 60.0--[[fixed timestep]], 10--[[iterations]])
    self.physics:Validate()
    self.physicsFrequency = 0.0
  end

  for k,v in pairs(self.entities) do
    v:update(dt,self)
  end
end


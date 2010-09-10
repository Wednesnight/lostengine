module("aqua", package.seeall)

require("lost.common.Class")
require("Player")
require("lost.common.Shaders")
require("Cloud")

local config = require("config")
local Color = lost.common.Color
local Rect = lost.math.Rect

lost.common.Class "World" {}


function World:constructor()
  self.entities = {}
  self.player = aqua.Player()
  
  tasklet.clearNode.active = false
  
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
  
  self:addEntity(aqua.Cloud())
end

function World:addEntity(ent)
  log.debug("!!!!!! adding entity: "..ent.name)
  table.insert(self.entities, ent)
  self.renderNode:add(ent.renderNode)
end

function World:updateEntitites(dt)
  log.debug("!!! starting entity update")
  for k,v in pairs(self.entitites) do
    print("updating entity "..v.name)
  end
  log.debug("!!! DONE entity update")
end


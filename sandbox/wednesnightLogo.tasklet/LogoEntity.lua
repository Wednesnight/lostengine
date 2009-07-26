require("Entity")

lost.common.Class "LogoEntity" "Entity" {}

local Rect = lost.math.Rect
local Vec3 = lost.math.Vec3
local Vec2 = lost.math.Vec2
local Color = lost.common.Color
local MatrixTranslation = lost.math.MatrixTranslation
local MatrixRotZ = lost.math.MatrixRotZ

function LogoEntity:create()
  Entity.create(self)
  self.font = dcl.font:TrueTypeFont
  {
    filename = "miserable.ttf",
    atlasSize = Vec2(512, 512)
  }
  self.mesh = dcl.font:Text
  {
    text = "Wednesnight",
    font = self.font,
    size = 60
  }
  self.rootNode = dcl.rg:Draw
  {
    mesh = self.mesh
  }
  self.angle = 0
  self.logoTranslationMatrix = MatrixTranslation(Vec3(200,200,0))  
end

function LogoEntity:update(tasklet, dt, at)
  self.angle = math.fmod(dt*.3+self.angle, 360)
  self.mesh.modelTransform = self.logoTranslationMatrix*MatrixRotZ(self.angle)
end

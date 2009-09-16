require("lost.declarative.Context")
require("Animator")
require("LogoEntity")
require("Fullscreen2DEntity")

local WindowParams = lost.application.WindowParams
local Rect = lost.math.Rect
local Vec3 = lost.math.Vec3
local Vec2 = lost.math.Vec2
local Color = lost.common.Color
local MatrixTranslation = lost.math.MatrixTranslation
local MatrixRotZ = lost.math.MatrixRotZ

windowRect = Rect(200,200,640,480)
windowParams = WindowParams("logo test", windowRect)

local exportScreens = false
local running = true

function startup(tasklet)
  dcl = lost.declarative.Context(tasklet.loader)
  animator = Animator()
  logoEntity = LogoEntity()
  rootEntity = Fullscreen2DEntity(Color(0,0,0,1))
  rootEntity:add(logoEntity)
  return true
end

function update(tasklet)
  if not exportScreens then
    animator:update(tasklet, rootEntity)
  else
    animator:dump(tasklet, rootEntity, 24, 10*1000, "/Users/tony/screens/")  
    running = false
  end
  return running
end

function shutdown(tasklet)
  return true
end

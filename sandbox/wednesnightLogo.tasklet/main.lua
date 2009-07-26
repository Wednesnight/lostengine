require("lost.declarative.Context")

local WindowParams = lost.application.WindowParams
local Rect = lost.math.Rect
local Vec3 = lost.math.Vec3
local Vec2 = lost.math.Vec2
local Color = lost.common.Color
local MatrixTranslation = lost.math.MatrixTranslation
local MatrixRotZ = lost.math.MatrixRotZ

hasWindow = true
windowRect = Rect(200,200,640,480)
windowParams = WindowParams("logo test", windowRect)

backgroundColor = Color(0,0,0,1)

local angle = 13
local startTime = 0
local lastUpdate = 0

local logoTranslationMatrix = MatrixTranslation(Vec3(200,200,0))

local exportScreens = false
local running = true

function startup(tasklet)
  dcl = lost.declarative.Context(tasklet.loader)

  titleFont = dcl.font:TrueTypeFont
  {
    filename = "miserable.ttf",
    atlasSize = Vec2(512,512)
  }
  
  renderedText = dcl.font:Text
  {
    text = "Wednesnight",
    size = 60,
    font = titleFont,
    transform = MatrixTranslation(Vec3(50,50,0))
  }
  
  rootNode = dcl.rg:Node
  {
    dcl.rg:ClearColor{color = backgroundColor},
    dcl.rg:Clear{mask = gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT},
    dcl.rg:Camera2D
    {
      viewport = Rect(0,0,windowRect.width,windowRect.height)
    },
    dcl.rg:DepthTest
    {
      false
    },
    dcl.rg:Draw
    {
      mesh = renderedText
    } 
  }
  
  lastUpdate = lost.platform.currentTimeMilliSeconds()
  startTime = lastUpdate
  return true
end

function doUpdate(tasklet, dt)
  rootNode:process(tasklet.window.context)
  angle = math.fmod(dt*.3+angle, 360)
  renderedText.modelTransform = logoTranslationMatrix*MatrixRotZ(angle)
  tasklet.window.context:swapBuffers()
end

function update(tasklet)
  local currentTime = lost.platform.currentTimeMilliSeconds()
  local dt = currentTime - lastUpdate 

  if not exportScreens then
    doUpdate(tasklet, dt)
  else
    local fps = 24
    local totalTime = 10000 -- msec
    local stepDelta = 1000 / fps
    local numFrames = totalTime / stepDelta
    local screenShotPath="/Users/tony/screenshots/"
    local filename = nil
    for i=1,numFrames do
      doUpdate(tasklet, stepDelta)
      filename = screenShotPath..i..".tga" 
      log.debug(filename)
      tasklet.window.context:writeScreenshot(filename, false)
    end
    running = false
  end
  
  lastUpdate = currentTime
  
  return running
end

function shutdown(tasklet)
  return true
end

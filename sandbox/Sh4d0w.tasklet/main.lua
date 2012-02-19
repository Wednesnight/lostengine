config = require("config")
require("scene")

local KeyEvent = lost.application.KeyEvent
local K_ESCAPE = lost.application.K_ESCAPE
local K_SPACE = lost.application.K_SPACE
local K_UP = lost.application.K_UP
local K_DOWN = lost.application.K_DOWN
local K_LEFT = lost.application.K_LEFT
local K_RIGHT = lost.application.K_RIGHT
local ResizeEvent = lost.application.ResizeEvent
local Rect = lost.math.Rect
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3

scene = nil -- deliberately global so ui can access it
screen = nil

local lightingEnabled = true
local shadowmapEnabled = true
local matcapEnabled = true

function updateButtons()
  screen("buttons")("stack")("ssao"):pushed(scene.ssaoEnabled)
  screen("buttons")("stack")("light"):pushed(scene.lightingEnabled)
  screen("buttons")("stack")("shadow"):pushed(scene.shadowmapEnabled)
  screen("buttons")("stack")("matcap"):pushed(scene.matcapEnabled)
end

function startup()
  require("lost.guiro")
  screen = require("ui")
--  lost.guiro.ui():add({screen})
  scene = createScene(tasklet.loader)

  tasklet.eventDispatcher:addEventListener(ResizeEvent.TASKLET_WINDOW_RESIZE, resize)

  matcapLoader = lost.resource.Loader.create()
  matcapLoader:addRepository(lost.resource.FilesystemRepository.create("/"))
  tasklet.eventDispatcher:addEventListener(lost.application.DragNDropEvent.DROP, function(event)
    scene:setMatCap(matcapLoader:load(event.filename))
  end)

  updateButtons()
end

function update()
  scene.firstPass:process(tasklet.window.context)
  if scene.secondPass.node.active then
    scene.secondPass:process(tasklet.window.context)
  end
  if scene.thirdPass.node.active then
    scene.thirdPass:process(tasklet.window.context, ssao)
  end
end

function key(event)
  if event.type == KeyEvent.KEY_DOWN then
    local updateCam = false
    if event.key == K_ESCAPE then
      tasklet.running = false
    elseif event.key == K_SPACE then
      scene.debugNode.active = not scene.debugNode.active
      scene.secondPass.node.active = not scene.secondPass.node.active
      scene.thirdPass.node.active = not scene.thirdPass.node.active
    elseif event.character == "1" then
      scene.ssaoEnabled = not scene.ssaoEnabled
      updateButtons()
    elseif event.character == "2" then
      scene.lightingEnabled = not scene.lightingEnabled
      updateButtons()
    elseif event.character == "3" then
      scene.shadowmapEnabled = not scene.shadowmapEnabled
      updateButtons()
    elseif event.character == "4" then
      scene.matcapEnabled = not scene.matcapEnabled
      updateButtons()
    elseif event.character == "w" then
      scene.cam.cam:rotate(Vec3(2,0,0))
    elseif event.character == "s" then
      scene.cam.cam:rotate(Vec3(-2,0,0))
    elseif event.character == "a" then
      scene.cam.cam:rotate(Vec3(0,2,0))
    elseif event.character == "d" then
      scene.cam.cam:rotate(Vec3(0,-2,0))
    elseif event.key == K_UP then
      scene.lightCam.cam:move(Vec3(0,0,0.1))
      updateCam = true
    elseif event.key == K_DOWN then
      scene.lightCam.cam:move(Vec3(0,0,-0.1))
      updateCam = true
    elseif event.key == K_LEFT then
      scene.lightCam.cam:move(Vec3(0.1,0,0))
      updateCam = true
    elseif event.key == K_RIGHT then
      scene.lightCam.cam:move(Vec3(-0.1,0,0))
      updateCam = true
    end
  
    -- update shader params
    if updateCam then
      scene.shaderParams:set("lightViewMatrix", scene.lightCam.cam:viewMatrix())
      scene.shaderParams:set("lightProjectionMatrix", scene.lightCam.cam:projectionMatrix())
    end
  end
end

function resize(event)  
  -- update cams
  scene.cam.cam:viewport(Rect(0, 0, event.width, event.height))
  scene.lightCam.cam:viewport(Rect(0, 0, event.width, event.height))
  scene.debugCam.cam:viewport(Rect(0, 0, event.width, event.height))
  -- ...framebuffers
  scene.fb:resize(Vec2(event.width, event.height))
  scene.fbSsao:resize(Vec2(event.width, event.height))
  -- ...and quads
  scene.debugMesh:updateSize(Vec2(event.width, event.height))
  
  -- update shader params
  scene.shaderParams:set("lightViewMatrix", scene.lightCam.cam:viewMatrix())
  scene.shaderParams:set("lightProjectionMatrix", scene.lightCam.cam:projectionMatrix())
end

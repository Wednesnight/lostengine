local _meta = require("_meta")
require("scene")

using "lost.application.KeyEvent"
using "lost.application.K_ESCAPE"
using "lost.application.K_SPACE"
using "lost.application.K_UP"
using "lost.application.K_DOWN"
using "lost.application.K_LEFT"
using "lost.application.K_RIGHT"
using "lost.application.ResizeEvent"
using "lost.math.Rect"
using "lost.math.Vec2"
using "lost.math.Vec3"

local scene = nil
local running = true
local debug = false

local lightingEnabled = true
local shadowmapEnabled = true

function startup(tasklet)
  tasklet.name = _meta.name
  tasklet.clearNode.active = false

  scene = createScene(tasklet.loader)
  tasklet.eventDispatcher:addEventListener(KeyEvent.KEY_DOWN, function(event)
    local updateCam = false
    if event.key == K_ESCAPE then
      running = false
    elseif event.key == K_SPACE then
      debug = not debug
    elseif event.character == "1" then
      scene.thirdPass.ssaoEnabled = not scene.thirdPass.ssaoEnabled
    elseif event.character == "2" then
      lightingEnabled = not lightingEnabled
      scene.ssaoShader:enable()
      scene.ssaoShader:setBool("lightingEnabled", lightingEnabled)
      scene.ssaoShader:disable()
    elseif event.character == "3" then
      shadowmapEnabled = not shadowmapEnabled
      scene.ssaoShader:enable()
      scene.ssaoShader:setBool("shadowmapEnabled", shadowmapEnabled)
      scene.ssaoShader:disable()
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
      scene.ssaoShader:enable()
      scene.ssaoShader:set("lightViewMatrix", scene.lightCam.cam:viewMatrix())
      scene.ssaoShader:set("lightProjectionMatrix", scene.lightCam.cam:projectionMatrix())
      scene.ssaoShader:disable()
    end
  end)
  tasklet.eventDispatcher:addEventListener(ResizeEvent.MAIN_WINDOW_RESIZE, function(event)
    
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
    scene.ssaoShader:enable()
    scene.ssaoShader:set("lightViewMatrix", scene.lightCam.cam:viewMatrix())
    scene.ssaoShader:set("lightProjectionMatrix", scene.lightCam.cam:projectionMatrix())
    scene.ssaoShader:disable()
  end)

  return true
end

function update(tasklet)
  scene.firstPass:process(tasklet.window.context)
  -- shadow map debug output?
  if debug then
    scene.debugNode:process(tasklet.window.context)
  else
    scene.secondPass:process(tasklet.window.context)
    scene.thirdPass:process(tasklet.window.context, ssao)
  end
  return running
end

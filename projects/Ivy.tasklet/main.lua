-- Ivy main.lua
require("lost.declarative.Context")

local Bounds = lost.guiro.Bounds
local xabs = lost.guiro.xabs
local yabs = lost.guiro.yabs
local wabs = lost.guiro.wabs
local habs = lost.guiro.habs
local wrel = lost.guiro.wrel
local hrel = lost.guiro.hrel

require("sizes")

windowParams = lost.application.WindowParams("Ivy", lost.math.Rect(200,200,640,480))
dcl = nil
screen = nil

function keyHandler(event)
  if event.key == lost.application.K_ESCAPE then
    running = false
  end
end

function startup(tasklet)
  tasklet.waitForEvents = true
  dcl = lost.declarative.Context(tasklet.loader)
  screen = require("ui")
  screen.bounds = Bounds(xabs(0), yabs(0), wabs(windowParams.rect.width), habs(windowParams.rect.height))
  screen:listenTo(tasklet.eventDispatcher)
  callLater(function() screen:updateLayout(true) end) 
  running = true
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)
  return true
end

function update(tasklet)
  processCallLaterQueue()
  screen:updateLayout(false)
  screen.rootNode:process(tasklet.window.context)
  tasklet.window.context:swapBuffers()
  return running
end

function shutdown(tasklet)
  return true
end
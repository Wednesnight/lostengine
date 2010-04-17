local _meta = require("_meta")
require("scene")

using "lost.application.KeyEvent"
using "lost.application.K_ESCAPE"
using "lost.application.K_SPACE"
using "lost.application.ResizeEvent"
using "lost.math.Rect"
using "lost.math.Vec2"

local scene = nil
local running = true
local debug = false

function startup(tasklet)
  tasklet.name = _meta.name
--  tasklet.waitForEvents = true

  scene = createScene(tasklet.loader)
  tasklet.eventDispatcher:addEventListener(KeyEvent.KEY_DOWN, function(event)
    if (event.key == K_ESCAPE) then
      running = false
    elseif (event.key == K_SPACE) then
      debug = not debug
    end
  end)
  tasklet.eventDispatcher:addEventListener(ResizeEvent.MAIN_WINDOW_RESIZE, function(event)
    scene.fb:resize(Vec2(event.width, event.height))
    local camNode = scene.rg:recursiveFindByName("cam")
    if camNode ~= nil then
      camNode.cam:viewport(Rect(0, 0, event.width, event.height))
    end
    camNode = scene.debugNode:recursiveFindByName("cam")
    if camNode ~= nil then
      camNode.cam:viewport(Rect(0, 0, event.width, event.height))
    end
    local quad = scene.debugNode:recursiveFindByName("quad")
    if quad ~= nil then
      quad.mesh:updateSize(Vec2(event.width, event.height))
    end
  end)

  return true
end

function update(tasklet)
  scene.firstPass:setup()
  scene.firstPass:process(tasklet.window.context)
  if debug then
    scene.debugNode:process(tasklet.window.context)
  else
    scene.secondPass:setup()
    scene.secondPass:process(tasklet.window.context)
  end
  return running
end

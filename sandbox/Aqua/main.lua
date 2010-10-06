require("Entity")
require("World")
require("lost.declarative.Context")

local running = true
local prevtime = 0
Vec2 = lost.math.Vec2

function keyHandler(event)
  if event.key == lost.application.K_ESCAPE then
    running = false
  elseif event.key == lost.application.K_A then
    world.player:left()
  elseif event.key == lost.application.K_D then
    world.player:right()
  elseif event.key == lost.application.K_W then
    world.player:jump()
  end
end

function startup()
  dcl = lost.declarative.Context(tasklet.loader)
  world = aqua.World()
  running = true
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)
  prevtime = lost.platform.currentTimeSeconds()
  time = 1.0 / 60.0
    
  return running
end

function update(deltaSeconds)
  world:updateEntities(deltaSeconds)
  return running
end

function shutdown()
end

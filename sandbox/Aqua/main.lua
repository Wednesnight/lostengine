require("Entity")
require("World")
require("PhysicsManager")
require("lost.declarative.Context")

local running = true
local prevtime = 0


function keyHandler(event)
  if event.key == lost.application.K_ESCAPE then
    running = false
  end
end

function startup()
  dcl = lost.declarative.Context(tasklet.loader)
  world = World()
  physicsManager = PhysicsManager()
  running = true
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)
  prevtime = lost.platform.currentTimeSeconds()
  
  return running
end

function update()
  local curtime = lost.platform.currentTimeSeconds()
  dt = (curtime - prevtime)
--  world:updateEntities(dt)
  prevtime = curtime
  return running
end

function shutdown()
end

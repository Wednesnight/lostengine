require("Entity")
require("World")
require("PhysicsManager")
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
  physicsManager = aqua.PhysicsManager()
  running = true
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)
  prevtime = lost.platform.currentTimeSeconds()
  time = 1.0 / 60.0
    
  return running
end

function update()
  local curtime = lost.platform.currentTimeSeconds()
  dt = (curtime - prevtime)

  local maxSteps = 1
  local fixedStep = 1.0 / 60.0

  local numSteps = 0
  if maxSteps > 0 then
    time = time + dt
    if time >= fixedStep then
      numSteps = math.floor(time / fixedStep)
    end
  else
    fixedStep = dt
    time = dt
    if math.abs(dt) < lost.math.EPSILON then
      numSteps = 0
      maxSteps = 0
    else
      numSteps = 1
      maxSteps = 1
    end
  end

  if numSteps > 0 then

    local clampedSteps = numSteps
    if numSteps > maxSteps then
      clampedSteps = maxSteps
    end

    for i = 1, clampedSteps do
      world:updateEntities(fixedStep)
    end
  end

  prevtime = curtime
  return running
end

function shutdown()
end

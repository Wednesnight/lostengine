function startup()
  local gravity = box2d.b2Vec2(0,-10)
  local doSleep = true
  world = box2d.b2World(gravity, doSleep)
end

function update()
  
end

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end
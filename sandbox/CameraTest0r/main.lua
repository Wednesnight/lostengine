require("lost.io.Rg")

gameState = {}

function startup(tasklet)
  gameState.mouseEvent = tasklet.eventDispatcher:addEventListener(lost.application.MouseEvent.MOUSE_MOVE, mouseMoved)

  local l = lost.io.Loader(tasklet.loader)
  scene = l:loadScene("testscene.lua")

  return true
end

function shutdown(tasklet)
  tasklet.eventDispatcher:removeEventListener(gameState.mouseEvent)
end

function mouseMoved(event)
  local mouseEvent = lost.application.MouseEvent.cast(event)
  
  gameState.light.rect.x = mouseEvent.pos.x - gameState.light.rect.width/2
  gameState.light.rect.y = mouseEvent.pos.y - gameState.light.rect.height/2
  gameState.light.mesh.modelTransform = lost.math.MatrixTranslation(lost.math.Vec3(gameState.light.rect.x, gameState.light.rect.y, 0.0));
end

function update(delta)
  for k,thing in next,gameState.things do
    if thing.rect:intersects(gameState.light.rect) then
      thing.rect.x = math.fmod(delta*50+thing.rect.x, 640)
    end
    thing.mesh.modelTransform = lost.math.MatrixTranslation(lost.math.Vec3(thing.rect.x, thing.rect.y, 0))
  end
end

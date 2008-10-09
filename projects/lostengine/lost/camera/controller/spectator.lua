require("Application")

lost.camera.CameraController = 
{
  camera = lost.camera.Camera(),
  dispatcher = nil
}

function lost.camera.CameraController:init(eventDispatcher)
  self.camera:position(lost.math.Vec3(0,3,15))
  self.camera:target(lost.math.Vec3(0,3,0))
  self.camera:stickToTarget(false)

  self.dispatcher = eventDispatcher

  self:initCallbacks()
end

function lost.camera.CameraController:initCallbacks()
--  self.dispatcher:addEventListenerMethod(lost.application.MouseEvent.MOUSE_MOVE, self, self.moveHandler)
--  self.dispatcher:addEventListenerMethod(lost.application.KeyEvent.KEY_DOWN, self, self.keyHandler)
  self.dispatcher:addEventListener(lost.application.MouseEvent.MOUSE_MOVE, self, self.moveHandler)
  self.dispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, self, self.keyHandler)
end

function lost.camera.CameraController:moveHandler(event)
  local moveEvent = lost.application.MouseEvent.cast(event)

  if (not self.moveInitialized) then
    self.mousePos        = moveEvent.pos
    self.moveInitialized = true
  else
    -- x-axis rotation
    local dx = -1.0 * (moveEvent.pos.y - self.mousePos.y) * 0.1
    -- y-axis rotation
    local dy = (moveEvent.pos.x - self.mousePos.x) * 0.1
    
    self.camera:rotate(lost.math.Vec3(dx, dy, 0.0))
    self.mousePos = moveEvent.pos
  end
end

function lost.camera.CameraController:keyHandler(event)
  local keyEvent = lost.application.KeyEvent.cast(event)

  if (keyEvent.key == lost.application.K_W) then
    self.camera:move(lost.math.Vec3(0, 0, -0.25))
  elseif (keyEvent.key == lost.application.K_A) then
    self.camera:move(lost.math.Vec3(-0.25, 0, 0))
  elseif (keyEvent.key == lost.application.K_S) then
    self.camera:move(lost.math.Vec3(0, 0, 0.25))
  elseif (keyEvent.key == lost.application.K_D) then
    self.camera:move(lost.math.Vec3(0.25, 0, 0))
  elseif (keyEvent.key == lost.application.K_Q) then
    self.camera:move(lost.math.Vec3(0, -0.25, 0))
  elseif (keyEvent.key == lost.application.K_E) then
    self.camera:move(lost.math.Vec3(0, 0.25, 0))
  end
end

lost.camera.CameraController_mt = { __index = lost.camera.CameraController }

function lost.camera.CameraController(eventDispatcher)
  local result = {}
  setmetatable(result, lost.camera.CameraController_mt)
  result:init(eventDispatcher)
  return result
end

lost.camera.CameraController = 
{
  camera  = lost.camera.Camera(),
  lastTap = 0.0,
  lastPos = lost.math.Vec2()
}

function lost.camera.CameraController:init()
  self.camera:position(lost.math.Vec3(0,3,30));
  self.camera:target(lost.math.Vec3(0,3,0));
  self.camera:stickToTarget(true);

  self:initCallbacks()
end

function lost.camera.CameraController:initCallbacks()
  globals.app:addEventListener(lost.application.AccelerometerEvent.DEVICE_ACCELERATED, self, self.accelerate)
  globals.app:addEventListener(lost.application.TouchEvent.TOUCHES_BEGAN, self, self.touchHandler)
  globals.app:addEventListener(lost.application.TouchEvent.TOUCHES_MOVED, self, self.touchHandler)
  globals.app:addEventListener(lost.application.TouchEvent.TOUCHES_ENDED, self, self.touchHandler)
  globals.app:addEventListener(lost.application.TouchEvent.TOUCHES_CANCELLED, self, self.touchHandler)
end

function lost.camera.CameraController:accelerate(event)
  local accelerometerEvent = lost.application.AccelerometerEvent.cast(event)

  local x = 0.0
  local y = 0.0
  local z = 0.0

  if (accelerometerEvent.y > 0.1 or accelerometerEvent.y < -0.1) then
    x = accelerometerEvent.y*0.5
  end
  if (accelerometerEvent.x > 0.1 or accelerometerEvent.x < -0.1) then
    y = -1*accelerometerEvent.x*0.5
  end

  self.camera:rotate(lost.math.Vec3(x, y, z));
end

function lost.camera.CameraController:touchHandler(event)
  local touchEvent = lost.application.TouchEvent.cast(event)
  if (touchEvent:size() == 1) then
    local touch = touchEvent:get(0)
    if (event.type == lost.application.TouchEvent.TOUCHES_BEGAN) then
      self.lastPos.x = touch.location.x
      self.lastPos.y = touch.location.y
      self.lastPos.z = touch.location.z
    elseif (event.type == lost.application.TouchEvent.TOUCHES_MOVED) then
      local deltaMove = lost.math.Vec3(0, 0, (touch.location.y - self.lastPos.y) * 0.1)
      self.camera:move(deltaMove)
      self.lastPos.x = touch.location.x
      self.lastPos.y = touch.location.y
      self.lastPos.z = touch.location.z
    end
  end
end

lost.camera.CameraController_mt = { __index = lost.camera.CameraController }

function lost.camera.CameraController()
  local result = {}
  setmetatable(result, lost.camera.CameraController_mt)
  return result
end

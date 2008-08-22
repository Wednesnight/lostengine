CameraController = 
{
  camera = lost.camera.Camera()
}

function CameraController:init()
  self.camera:position(lost.math.Vec3(0,3,15));
  self.camera:target(lost.math.Vec3(0,3,0));
  self.camera:stickToTarget(true);

  self:initCallbacks()
end

function CameraController:initCallbacks()
  local AccelerometerEvent = lost.application.AccelerometerEvent
  app:addEventListener(AccelerometerEvent.DEVICE_ACCELERATED, self.accelerate)

  local TouchEvent = lost.application.TouchEvent
  app:addEventListener(TouchEvent.TOUCHES_BEGAN, self.touch)
  app:addEventListener(TouchEvent.TOUCHES_MOVED, self.touch)
  app:addEventListener(TouchEvent.TOUCHES_ENDED, self.touch)
  app:addEventListener(TouchEvent.TOUCHES_CANCELLED, self.touch)
end

function CameraController:accelerate(event)
  local accelerometerEvent = lost.application.AccelerometerEvent.cast(event)

  local x = 0.0
  local y = 0.0
  local z = 0.0

  if (accelerometerEvent.y > 0.1) then
    x = accelerometerEvent.y*0.5
  end
  if (accelerometerEvent.x > 0.1) then
    y = accelerometerEvent.x*0.5
  end

  self.camera:rotate(lost.math.Vec3(x, y, z));
end

function CameraController:touch(event)
  log.debug(event.type)
  local touchEvent = lost.application.TouchEvent.cast(event)
end

CameraController_mt = {__index = CameraController}

function CameraController:new()
  local result = {}
  setmetatable(result, CameraController_mt)
  return result
end

lost.camera.CameraController = 
{
  camera  = lost.camera.Camera(),
  lastTap = 0.0;
}

function lost.camera.CameraController:init()
  self.camera:position(lost.math.Vec3(0,3,30));
  self.camera:target(lost.math.Vec3(0,3,0));
  self.camera:stickToTarget(true);

  self:initCallbacks()
end

function lost.camera.CameraController:initCallbacks()
  local AccelerometerEvent = lost.application.AccelerometerEvent
  globals.app:addEventListener(AccelerometerEvent.DEVICE_ACCELERATED, self, self.accelerate)
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

lost.camera.CameraController_mt = { __index = lost.camera.CameraController }

function lost.camera.CameraController()
  local result = {}
  setmetatable(result, lost.camera.CameraController_mt)
  return result
end

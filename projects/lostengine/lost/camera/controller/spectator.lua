lost.camera.CameraController = 
{
  camera = lost.camera.Camera()
}

function lost.camera.CameraController:init()
  self.camera:position(lost.math.Vec3(0,3,15));
  self.camera:target(lost.math.Vec3(0,3,0));
  self.camera:stickToTarget(false);

  self:initCallbacks()
end

function lost.camera.CameraController:initCallbacks()
end

lost.camera.CameraController_mt = { __index = lost.camera.CameraController }

function lost.camera.CameraController()
  local result = {}
  setmetatable(result, lost.camera.CameraController_mt)
  result:init()
  return result
end

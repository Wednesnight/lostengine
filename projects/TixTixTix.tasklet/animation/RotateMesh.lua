module("animation", package.seeall)

require("animation.Animation")

lost.common.Class "animation.RotateMesh" "animation.Animation" {}

function RotateMesh:constructor(mesh, durationInSeconds, rotationAsVec3)
  Animation.constructor(self)

  self.mesh = mesh
  self.targetDuration = durationInSeconds
  self.currentDuration = 0
  self.targetRotation = rotationAsVec3
  self.currentRotation = lost.math.Vec3(0,0,0)
  self.factorRotation = self.targetRotation * (1 / self.targetDuration)
  self.initialized = false
end

function RotateMesh:reset()
  Animation.reset(self)
  if self.targetRotation ~= nil then
    self.currentRotation = lost.math.Vec3(0,0,0)
    if self.reverse then
      self.targetRotation = self.targetRotation * -1
      self.factorTranslation = self.targetRotation * (1 / self.targetDuration)
    end
  end
  self.initialized = false
  self.currentDuration = 0
end

function RotateMesh:update(deltaSec)

  if self.initialized then
    self.currentDuration = self.currentDuration + deltaSec
  else
    self.initialized = true
  end

  local rotation = self:checkBounds(self.targetRotation, self.factorRotation * self.currentDuration)
  local rotationDelta = rotation - self.currentRotation
  self.currentRotation = rotation
  local matrix = lost.math.MatrixRotX(rotationDelta.x) * lost.math.MatrixRotY(rotationDelta.y) * lost.math.MatrixRotZ(rotationDelta.z)
  self.mesh.transform = self.mesh.transform * matrix

  return (self.currentRotation == self.targetRotation)
end

function RotateMesh:checkBounds(target, current)
  if (target.x < 0) then
    current.x = math.max(target.x, current.x)
  else
    current.x = math.min(target.x, current.x)
  end
  if (target.y < 0) then
    current.y = math.max(target.y, current.y)
  else
    current.y = math.min(target.y, current.y)
  end
  if (target.z < 0) then
    current.z = math.max(target.z, current.z)
  else
    current.z = math.min(target.z, current.z)
  end
  return current
end

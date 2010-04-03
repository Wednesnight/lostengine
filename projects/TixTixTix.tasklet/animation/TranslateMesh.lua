module("animation", package.seeall)

require("animation.Animation")

lost.common.Class "animation.TranslateMesh" "animation.Animation" {}

function TranslateMesh:constructor(mesh, durationInSeconds, translationAsVec3)
  Animation.constructor(self)

  self.mesh = mesh
  self.targetDuration = durationInSeconds
  self.currentDuration = 0
  self.targetTranslation = translationAsVec3
  self.currentTranslation = lost.math.Vec3(0,0,0)
  self.factorTranslation = self.targetTranslation * (1 / self.targetDuration)
  self.initialized = false
end

function TranslateMesh:reset()
  Animation.reset(self)
  self.currentTranslation = lost.math.Vec3(0,0,0)
  self.initialized = false
  self.currentDuration = 0
end

function TranslateMesh:reverse()
  if self.targetTranslation ~= nil then
    self.targetTranslation = self.targetTranslation * -1
    self.factorTranslation = self.targetTranslation * (1 / self.targetDuration)
  end
end

function TranslateMesh:update(deltaSec)

  if self.initialized then
    self.currentDuration = self.currentDuration + deltaSec
  else
    self.initialized = true
  end

  local translation = self:checkBounds(self.targetTranslation, self.factorTranslation * self.currentDuration)
  local translationDelta = translation - self.currentTranslation
  self.currentTranslation = translation
  local matrix = lost.math.MatrixTranslation(translationDelta)
  self.mesh.transform = self.mesh.transform * matrix

  return (self.currentTranslation == self.targetTranslation)
end

function TranslateMesh:checkBounds(target, current)
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

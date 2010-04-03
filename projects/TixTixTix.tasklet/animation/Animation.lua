module("animation", package.seeall)

require("lost.common.Class")

lost.common.Class "animation.Animation" {}

function Animation:constructor()
  self.running = false
end

function Animation:run()
  self.running = true
  self.passedSec = lost.platform.currentTimeSeconds()
  lost.common.callLater(Animation.process, self)
end

function Animation:stop()
  self.running = false
end

function Animation:reset()
  self.running = false
end

function Animation:process()
  local currentSec = lost.platform.currentTimeSeconds()
  local deltaSec = currentSec - self.passedSec
  if not self:update(deltaSec) and self.running then
    lost.common.callLater(Animation.process, self)
    self.passedSec = currentSec
  elseif self.running then
    self:reset()
    if self.onFinished ~= nil then
      self.onFinished(self)
    end
  end
end

function Animation:update(deltaSec)
  return true
end

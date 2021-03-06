module("lost.animation", package.seeall)

require("lost.common.Class")
require("lost.animation.Interpolation")
require("lost.common.CallLater")

lost.common.Class "lost.animation.Tween" {}

Tween.linear = Interpolation.linear
Tween.sineIn = Interpolation.sineIn
Tween.sineOut = Interpolation.sineOut
Tween.sineInOut = Interpolation.sineInOut
Tween.quadIn = Interpolation.quadIn
Tween.quadOut = Interpolation.quadOut
Tween.quadInOut = Interpolation.quadInOut
Tween.cubicIn = Interpolation.cubicIn
Tween.cubicOut = Interpolation.cubicOut
Tween.cubicInOut = Interpolation.cubicInOut
Tween.quartIn = Interpolation.quartIn
Tween.quartOut = Interpolation.quartOut
Tween.quartInOut = Interpolation.quartInOut
Tween.quintIn = Interpolation.quintIn
Tween.quintOut = Interpolation.quintOut
Tween.quintInOut = Interpolation.quintInOut
Tween.expoIn = Interpolation.expoIn
Tween.expoOut = Interpolation.expoOut
Tween.expoInOut = Interpolation.expoInOut
Tween.backIn = Interpolation.backIn
Tween.backOut = Interpolation.backOut
Tween.backInOut = Interpolation.backInOut
Tween.elasticIn = Interpolation.elasticIn
Tween.elasticOut = Interpolation.elasticOut
Tween.elasticInOut = Interpolation.elasticInOut
Tween.circIn = Interpolation.circIn
Tween.circOut = Interpolation.circOut
Tween.circInOut = Interpolation.circInOut
Tween.bounceIn = Interpolation.bounceIn
Tween.bounceOut = Interpolation.bounceOut
Tween.bounceInOut = Interpolation.bounceInOut

function Tween:constructor(target, targetProperties, tweenProperties, autoStart)
  tweenProperties = tweenProperties or {}
  tweenProperties.duration = tweenProperties.duration or 1
  tweenProperties.method = tweenProperties.method or Tween.linear

  local originalProperties = {}
  for property in next, targetProperties do
    originalProperties[property] = target[property]
  end

  self.target = target
  self.originalProperties = originalProperties
  self.targetProperties = targetProperties
  self.tweenProperties = tweenProperties
  self.time = 0
  self.timestamp = lost.platform.currentTimeSeconds()

  self.timer = tasklet.scheduler:createTimer(1/60, Tween.process, self)
  if autoStart == nil or autoStart then
    self.timer:start()
  end
end

function Tween:start()
  self.timer:start()
end

function Tween:stop()
  self.timer:stop()
end

function Tween:restart()
  for property in next, self.originalProperties do
    self.target[property] = self.originalProperties[property]
  end
  self.timer:restart()
end

function Tween:process(timer)
  self.time = math.min(self.time + timer:getInterval(), self.tweenProperties.duration)
  self.timestamp = now

  -- Since we always use 0 and 1, we could simplify the Interpolation functions
  -- instead.
  local delta = self.tweenProperties.method(self.time, 0, 1, self.tweenProperties.duration)

  if type(self.tweenProperties.beforeProcess) == "function" then
    self.tweenProperties.beforeProcess(self, delta)
  end

  for property in next, self.targetProperties do
    self.target[property] = self.originalProperties[property] + (delta * (self.targetProperties[property] - self.originalProperties[property]))
  end

  if type(self.tweenProperties.afterProcess) == "function" then
    self.tweenProperties.afterProcess(self, delta)
  end

  if self.time < self.tweenProperties.duration then
    return true
  else
    if type(self.tweenProperties.onComplete) == "function" then
      self.tweenProperties.onComplete(self)
    end
    return false
  end
end

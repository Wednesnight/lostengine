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

function Tween:constructor(target, targetProperties, tweenProperties)
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

  lost.common.callLater(Tween.process, self)
end

function Tween:process()
  local now = lost.platform.currentTimeSeconds()
  self.time = math.min(self.time + (now - self.timestamp), self.tweenProperties.duration)
  self.timestamp = now
  
  if type(self.tweenProperties.beforeProcess) == "function" then
    self.tweenProperties.beforeProcess(self)
  end

  for property in next, self.targetProperties do
    self.target[property] = self.tweenProperties.method(self.time,
                                                        self.originalProperties[property],
                                                        self.targetProperties[property] - self.originalProperties[property],
                                                        self.tweenProperties.duration)
  end

  if type(self.tweenProperties.afterProcess) == "function" then
    self.tweenProperties.afterProcess(self)
  end

  if self.time < self.tweenProperties.duration then
    lost.common.callLater(Tween.process, self)
  else
    if type(self.tweenProperties.onComplete) == "function" then
      self.tweenProperties.onComplete(self)
    end
  end
end

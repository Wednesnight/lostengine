module("lost.animation", package.seeall)

require("lost.common.Class")
require("animation.Animation")

lost.common.Class "lost.animation.AnimationHelper" {}

--[[
    xxxxxx  Interpolation  xxxxxx
    
    Rules:
      f(x) = f0 + ((f1 - f0) * (1 / x1 - x0)) * (x - x0)

    where:
      f() = interpolation function
      f0  = start value
      f1  = target value
      x0  = start time (typically 0) in seconds
      x1  = overall duration in seconds
      x   = current time in seconds
  ]]

function AnimationHelper.interpolateVec2(f0, f1, x1)
  local animation = AnimationHelper.setupInterpolation(f0, f1, x1)
  animation:addEventListener("animationUpdate", function(animation, deltaSec)
    if animation.currentTime ~= nil then
      animation.currentTime = animation.currentTime + deltaSec
    else
      animation.currentTime = animation.startTime
    end

    local value = animation.startValue + animation.factor * animation.currentTime
    if (animation.targetValue.x < animation.startValue.x) then
      value.x = math.max(animation.targetValue.x, value.x)
    else
      value.x = math.min(animation.targetValue.x, value.x)
    end
    if (animation.targetValue.y < animation.startValue.y) then
      value.y = math.max(animation.targetValue.y, value.y)
    else
      value.y = math.min(animation.targetValue.y, value.y)
    end
    animation.currentValue = value
    return (animation.currentValue == animation.targetValue)
  end)
  animation:addEventListener("animationFinished", function(animation)
    AnimationHelper.resetInterpolation(animation)
  end)
  return animation
end

function AnimationHelper.interpolateVec3(f0, f1, x1)
  local animation = AnimationHelper.setupInterpolation(f0, f1, x1)
  animation:addEventListener("animationUpdate", function(animation, deltaSec)
    if animation.currentTime ~= nil then
      animation.currentTime = animation.currentTime + deltaSec
    else
      animation.currentTime = animation.startTime
    end

    local value = animation.startValue + animation.factor * animation.currentTime
    if (animation.targetValue.x < animation.startValue.x) then
      value.x = math.max(animation.targetValue.x, value.x)
    else
      value.x = math.min(animation.targetValue.x, value.x)
    end
    if (animation.targetValue.y < animation.startValue.y) then
      value.y = math.max(animation.targetValue.y, value.y)
    else
      value.y = math.min(animation.targetValue.y, value.y)
    end
    if (animation.targetValue.z < animation.startValue.z) then
      value.z = math.max(animation.targetValue.z, value.z)
    else
      value.z = math.min(animation.targetValue.z, value.z)
    end
    animation.currentValue = value
    return (animation.currentValue == animation.targetValue)
  end)
  animation:addEventListener("animationFinished", function(animation)
    AnimationHelper.resetInterpolation(animation)
  end)
  return animation
end

function AnimationHelper.interpolateVec4(f0, f1, x1)
  local animation = AnimationHelper.setupInterpolation(f0, f1, x1)
  animation:addEventListener("animationUpdate", function(animation, deltaSec)
    if animation.currentTime ~= nil then
      animation.currentTime = animation.currentTime + deltaSec
    else
      animation.currentTime = animation.startTime
    end

    local value = animation.startValue + animation.factor * animation.currentTime
    if (animation.targetValue.x < animation.startValue.x) then
      value.x = math.max(animation.targetValue.x, value.x)
    else
      value.x = math.min(animation.targetValue.x, value.x)
    end
    if (animation.targetValue.y < animation.startValue.y) then
      value.y = math.max(animation.targetValue.y, value.y)
    else
      value.y = math.min(animation.targetValue.y, value.y)
    end
    if (animation.targetValue.z < animation.startValue.z) then
      value.z = math.max(animation.targetValue.z, value.z)
    else
      value.z = math.min(animation.targetValue.z, value.z)
    end
    if (animation.targetValue.w < animation.startValue.w) then
      value.w = math.max(animation.targetValue.w, value.w)
    else
      value.w = math.min(animation.targetValue.w, value.w)
    end
    animation.currentValue = value
    return (animation.currentValue == animation.targetValue)
  end)
  animation:addEventListener("animationFinished", function(animation)
    AnimationHelper.resetInterpolation(animation)
  end)
  return animation
end

--[[
    xxxxxx  Public interpolation helper methods  xxxxxx
  ]]

function AnimationHelper.resetInterpolation(animation)
  animation.currentValue = nil
  animation.startTime = 0
  animation.currentTime = nil
  animation.factor = ((animation.targetValue - animation.startValue) * (1 / animation.targetTime))
end

function AnimationHelper.reverseInterpolation(animation)
  local startValue = animation.startValue
  local targetValue = animation.targetValue
  animation.startValue = targetValue
  animation.targetValue = startValue
  animation.currentValue = nil
  animation.startTime = 0
  animation.currentTime = nil
  animation.factor = ((animation.targetValue - animation.startValue) * (1 / animation.targetTime))
end

--[[
    xxxxxx  Private interpolation helper methods  xxxxxx
  ]]

function AnimationHelper.setupInterpolation(f0, f1, x1)
  local result = Animation()
  result.startValue = f0
  result.targetValue = f1
  result.currentValue = nil
  result.startTime = 0
  result.targetTime = x1
  result.currentTime = nil
  result.factor = ((result.targetValue - result.startValue) * (1 / result.targetTime))
  return result
end

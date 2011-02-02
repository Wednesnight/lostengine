--[[
    see: http://www.robertpenner.com/easing/
  ]]

module("lost.animation", package.seeall)

require("lost.common.Class")

lost.common.Class "lost.animation.Interpolation" {}

function Interpolation.linear(time, original, change, duration)
  return change * time / duration + original
end

function Interpolation.sineIn(time, original, change, duration)
	return -change * math.cos(time / duration * (math.pi / 2)) + change + original
end

function Interpolation.sineOut(time, original, change, duration)
	return change * math.sin(time / duration * (math.pi / 2)) + original
end

function Interpolation.sineInOut(time, original, change, duration)
	return -change / 2 * (math.cos(math.pi * time / duration) - 1) + original
end

function Interpolation.quadIn(time, original, change, duration)
  time = time / duration
  return change * time * time + original
end

function Interpolation.quadOut(time, original, change, duration)
  time = time / duration
  return -change * time * (time - 2) + original
end

function Interpolation.quadInOut(time, original, change, duration)
  time = time / (duration / 2)
	if time < 1 then
	  return change / 2 * time * time + original
	else
	  time = time - 1
	  return -change / 2 * (time * (time - 2) - 1) + original
  end
end

function Interpolation.cubicIn(time, original, change, duration)
  time = time / duration
	return change * time * time * time + original
end

function Interpolation.cubicOut(time, original, change, duration)
  time = time / duration - 1
	return change * (time * time * time + 1) + original
end

function Interpolation.cubicInOut(time, original, change, duration)
  time = time / (duration / 2)
	if time < 1 then
	  return change / 2 * time * time * time + original
	else
	  time = time - 2
	  return change / 2 * (time * time * time + 2) + original
	end
end

function Interpolation.quartIn(time, original, change, duration)
  time = time / duration
	return change * time * time * time * time + original
end

function Interpolation.quartOut(time, original, change, duration)
  time = time / duration - 1
	return -change * (time * time * time * time - 1) + original
end

function Interpolation.quartInOut(time, original, change, duration)
  time = time / (duration / 2)
	if time < 1 then
	  return change / 2 * time * time * time * time + original
	else
	  time = time - 2
	  return -change / 2 * (time * time * time * time - 2) + original
	end
end

function Interpolation.quintIn(time, original, change, duration)
  time = time / duration
	return change * time * time * time * time * time + original
end

function Interpolation.quintOut(time, original, change, duration)
  time = time / duration - 1
	return change * (time * time * time * time * time + 1) + original
end

function Interpolation.quintInOut(time, original, change, duration)
  time = time / (duration / 2)
	if time < 1 then
	  return change / 2 * time * time * time * time * time + original
	else
	  time = time - 2
	  return change / 2 * (time * time * time * time * time + 2) + original
	end
end

function Interpolation.expoIn(time, original, change, duration)
  if time == 0 then
    return original
  else
    return change * math.pow(2, 10 * (time / duration - 1)) + original
  end
end

function Interpolation.expoOut(time, original, change, duration)
  if time == duration then
    return original + change
  else
	  return change * (-math.pow(2, -10 * time / duration) + 1) + original
	end
end

function Interpolation.expoInOut(time, original, change, duration)
	if time == 0 then
	  return original
	elseif time == duration then
	  return original + change
	else
	  time = time / (duration / 2)
	  if time < 1 then
	    return change / 2 * math.pow(2, 10 * (time - 1)) + original
	  else
	    time = time - 1
	    return change / 2 * (-math.pow(2, -10 * time) + 2) + original
	  end
	end
end

function Interpolation.backIn(time, original, change, duration, s)
	s = s or 1.70158
	time = time / duration
	return change * time * time * ((s + 1) * time - s) + original
end

function Interpolation.backOut(time, original, change, duration, s)
	s = s or 1.70158
	time = time / duration - 1
	return change * (time * time * ((s + 1) * time + s) + 1) + original
end

function Interpolation.backInOut(time, original, change, duration, s)
	s = s or 1.70158
	time = time / (duration / 2)
	if time < 1 then
	  s = s * 1.525
	  return change / 2 * (time * time * ((s + 1) * time - s)) + original
	else
	  s = s * 1.525
	  time = time - 2
	  return change / 2 * (time * time * ((s + 1) * time + s) + 2) + original
	end
end

function Interpolation.elasticIn(time, original, change, duration, a, p)
	if time == 0 then
	  return original
	else
	  time = time / duration
	  if time == 1 then
	    return original + change
	  else
	    time = time - 1
	    p = p or duration * 0.3
	    local s = 0
	    if a == nil or a < math.abs(change) then
	      a = change
	      s = p / 4
	    else
	      s = p / (2 * math.pi) * math.asin(change / a)
	    end
	    return -(a * math.pow(2, 10 * time) * math.sin((time * duration - s) * (2 * math.pi) / p)) + original
    end
  end
end

function Interpolation.elasticOut(time, original, change, duration)
	if time == 0 then
	  return original
	else
	  time = time / duration
	  if time == 1 then
	    return original + change
	  else
	    p = p or duration * 0.3
	    local s = 0
	    if a == nil or a < math.abs(change) then
	      a = change
	      s = p / 4
	    else
	      s = p / (2 * math.pi) * math.asin(change / a)
	    end
    	return a * math.pow(2, -10 * time) * math.sin((time * duration - s) * (2 * math.pi) / p) + change + original
    end
  end
end

function Interpolation.elasticInOut(time, original, change, duration)
	if time == 0 then
	  return original
	else
	  time = time / (duration / 2)
	  if time == 2 then
	    return original + change
	  else
	    p = p or duration * (0.3 * 1.5)
	    local s = 0
	    if a == nil or a < math.abs(change) then
	      a = change
	      s = p / 4
	    else
	      s = p / (2 * math.pi) * math.asin(change / a)
	    end
    	if time < 1 then
    	  time = time - 1
    	  return -0.5 * (a * math.pow(2, 10 * time) * math.sin((time * duration - s) * (2 * math.pi) / p)) + original
    	else
    	  time = time - 1
    	  return a * math.pow(2, -10 * time) * math.sin((time * duration - s) * (2 * math.pi) / p) * 0.5 + change + original
    	end
    end
  end
end

function Interpolation.circIn(time, original, change, duration)
  time = time / duration
	return -change * (math.sqrt(1 - time * time) - 1) + original
end

function Interpolation.circOut(time, original, change, duration)
  time = time / duration - 1
	return change * math.sqrt(1 - time * time) + original
end

function Interpolation.circInOut(time, original, change, duration)
  time = time / (duration / 2)
	if time < 1 then
	  return -change / 2 * (math.sqrt(1 - time * time) - 1) + original
  else
    time = time - 2
	  return change / 2 * (math.sqrt(1 - time * time) + 1) + original
  end
end

function Interpolation.bounceIn(time, original, change, duration)
	return change - Interpolation.bounceOut (duration - time, 0, change, duration) + original
end

function Interpolation.bounceOut(time, original, change, duration)
  time = time / duration
	if time < 1 / 2.75 then
		return change * (7.5625 * time * time) + original
	elseif time < 2 / 2.75 then
	  time = time - 1.5 / 2.75
		return change * (7.5625 * time * time + 0.75) + original
	elseif time < 2.5 / 2.75 then
	  time = time - 2.25 / 2.75
		return change * (7.5625 * time * time + 0.9375) + original
	else
	  time = time - 2.625 / 2.75
		return change * (7.5625 * time * time + 0.984375) + original
	end
end

function Interpolation.bounceInOut(time, original, change, duration)
  if time < duration / 2 then
    return Interpolation.bounceIn(time * 2, 0, change, duration) * 0.5 + original
  else
    return Interpolation.bounceOut(time * 2 - duration, 0, change, duration) * 0.5 + change * 0.5 + original
  end
end

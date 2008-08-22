function accelerate(event)
--  log.debug(event.type)
  local accelerometerEvent = lost.application.AccelerometerEvent.cast(event)
--  log.debug(accelerometerEvent.x ..", ".. accelerometerEvent.y ..", ".. accelerometerEvent.z)
--  log.debug(accelerometerEvent.timeStamp)
end
local AccelerometerEvent = lost.application.AccelerometerEvent
app:addEventListener(AccelerometerEvent.DEVICE_ACCELERATED, accelerate)

function touch(event)
  log.debug(event.type)
  local touchEvent = lost.application.TouchEvent.cast(event)
--  log.debug(touchEvent.touches:size())
end
local TouchEvent = lost.application.TouchEvent
app:addEventListener(TouchEvent.TOUCHES_BEGAN, touch)
app:addEventListener(TouchEvent.TOUCHES_MOVED, touch)
app:addEventListener(TouchEvent.TOUCHES_ENDED, touch)
app:addEventListener(TouchEvent.TOUCHES_CANCELLED, touch)

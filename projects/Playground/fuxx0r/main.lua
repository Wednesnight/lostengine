hasWindow = true
windowParams = lost.application.WindowParams("fuxx0r", lost.math.Rect(50,200,640,480))

function startup(tasklet)
  log.debug("startup")
  return true
end

function update(tasklet)
  log.debug("update")
  return true
end

function shutdown(tasklet)
  log.debug("shutdown")
  return true
end

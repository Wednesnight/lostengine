hasWindow = true
windowParams = lost.application.WindowParams("fuxx0r", lost.math.Rect(50,200,640,480))

local node = lost.rg.Node.create()

function startup(tasklet)
  log.debug("startup")
  return true
end

function update(tasklet)
--  log.debug("update")
  node:process(tasklet.window.context)
  return true
end

function shutdown(tasklet)
  log.debug("shutdown")
  return true
end

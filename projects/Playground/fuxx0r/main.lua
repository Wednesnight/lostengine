windowWidth = 640
windowHeight = 480
windowTitle = "fuxx0r"

function startup(tasklet)
  log.debug("startup")
  return true
end

function main(tasklet)
  log.debug("main")
  return true
end

function shutdown(tasklet)
  log.debug("shutdown")
  return true
end

local WindowParams = lost.application.WindowParams
local Rect = lost.math.Rect

require("lost.declarative.Context")

hasWindow = true
windowParams = WindowParams("Guiroscope", Rect(300,300,640,480))

ui = nil

function startup(tasklet)
  log.debug("starting up")
  tasklet.waitForEvents = true
  dcl = lost.declarative.Context(tasklet.loader)

  screen = dcl.guiro:Screen
  {
    dcl.guiro:UserInterface
    {
      dcl.guiro:Window
      {
        
      },
      dcl.guiro:Window
      {
        
      }
    }
  }

  screen:printSubviews()
  screen:listenTo(tasklet.eventDispatcher)
  return true
end

function update(tasklet)
  return true
end

function shutdown(tasklet)
  log.debug("shutting down")
  return true
end

local WindowParams = lost.application.WindowParams
local Rect = lost.math.Rect

require("lost.declarative.Context")

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

  local c = lost.common.Color(1,2,3,4)
  log.debug("color : "..tostring(c))

  screen:printSubviews()
  screen:listenTo(tasklet.eventDispatcher)
  screen.currentGlobalRect = lost.math.Rect(0,0,windowParams.rect.width, windowParams.rect.height)
  screen:updateLayout() --force update of layout
  return true
end

function update(tasklet)
  return true
end

function shutdown(tasklet)
  log.debug("shutting down")
  return true
end

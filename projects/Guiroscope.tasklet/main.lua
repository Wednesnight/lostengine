local WindowParams = lost.application.WindowParams
local Rect = lost.math.Rect

require("lost.declarative.Context")

windowParams = WindowParams("Guiroscope", Rect(300,300,640,480))

local Bounds = lost.guiro.Bounds
local xabs = lost.guiro.xabs
local yabs = lost.guiro.yabs
local xrel = lost.guiro.xrel
local yrel = lost.guiro.yrel
local wabs = lost.guiro.wabs
local habs = lost.guiro.habs
local wrel = lost.guiro.wrel
local hrel = lost.guiro.hrel
local Color = lost.common.Color

-- these are deliberately global so we can access them from startup/update/shutdown
dcl = nil
screen = nil

function startup(tasklet)
  log.debug("starting up")
  tasklet.waitForEvents = true
  dcl = lost.declarative.Context(tasklet.loader)

  screen = dcl.guiro:Screen
  {
    id = "screen",
    dcl.guiro:UserInterface
    {
      id = "mainUi",
      bounds = Bounds(xabs(0), yabs(0), wrel(1), hrel(1)),
      dcl.guiro:Window
      {
        id = "window1",
        bounds = Bounds(xabs(0), yabs(0), wrel(.5), hrel(.5)),
        showFrame = false,
        frameColor = Color(1,1,1),
        showBackground = true,
        backgroundColor = Color(1,1,0)
      },
      dcl.guiro:Window
      {
        id = "window2",
        bounds = Bounds(xrel(.5), yrel(.5), wrel(.5), hrel(.5)),
        showFrame = true,
        frameColor = Color(0,1,0),
        showBackground = false,
        backgroundColor = Color(1,0,0)
      }
    }
  }

  screen:printSubviews()
  screen:listenTo(tasklet.eventDispatcher)
  screen.currentGlobalRect = lost.math.Rect(0,0,windowParams.rect.width, windowParams.rect.height)
  screen:updateLayout() --force update of layout
  screen.rootNode:print()
  return true
end

function update(tasklet)
  screen.rootNode:process(tasklet.window.context)
  tasklet.window.context:swapBuffers()
  return true
end

function shutdown(tasklet)
  log.debug("shutting down")
  return true
end

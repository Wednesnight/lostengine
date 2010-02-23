local taskletName = "Shader Lab"

require("lost.declarative.Context")
require("lost.guiro.RenderView")

using "lost.application.WindowParams"
using "lost.math.Rect"
using "lost.declarative.Context"

windowParams = WindowParams(taskletName, Rect(200,200,640,480))

local controller = require("controller")

function startup(tasklet)
  controller.running = true

  tasklet.name = taskletName
  tasklet.waitForEvents = true
  
  dcl = Context(tasklet.loader)
  screen = require("ui")
--  screen("ui")("window")("renderView")("scene")._renderViewNode:print()

  return true
end

function update(tasklet)
  return controller.running
end

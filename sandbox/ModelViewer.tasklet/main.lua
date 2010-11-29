local taskletName = "Model Viewer"

require("lost.declarative.Context")
require("lost.guiro.view.RenderView")

using "lost.application.WindowParams"
using "lost.math.Rect"
using "lost.declarative.Context"

local controller = require("controller")

function startup()
  controller.running = true
  
  dcl = Context(tasklet.loader)
  screen = require("ui")

  return true
end

function update()
  return controller.running
end

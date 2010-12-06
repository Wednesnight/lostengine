require("lost.declarative.Context")
require("lost.guiro.view.RenderView")

using "lost.math.Rect"
using "lost.declarative.Context"

local controller = require("controller")

function startup()
  controller.running = true
  
  dcl = Context(tasklet.loader)
  screen = require("ui")
--  screen("ui")("window")("renderView")("scene")._renderViewNode:print()

  return true
end

function update()
  return controller.running
end

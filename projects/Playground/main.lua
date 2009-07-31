require("lost.declarative.Context")

local WindowParams = lost.application.WindowParams
local Rect = lost.math.Rect

hasWindow = true
windowParams = WindowParams("Playground",
                            Rect(50,800,400, 128))

function startup(tasklet)
  dcl = lost.declarative.Context(tasklet.loader)
  ui = require("ui")
  return true
end

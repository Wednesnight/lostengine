local taskletName = "ShaderLab"

require("lost.declarative.Context")

using "lost.application.WindowParams"
using "lost.math.Rect"
using "lost.declarative.Context"

windowParams = WindowParams(taskletName, Rect(200,200,640,480))
running = false

function startup(tasklet)
  running = true

  tasklet.name = taskletName
  tasklet.waitForEvents = true
  
  dcl = Context(tasklet.loader)
  require("ui")

  return true
end

function update(tasklet)
  return running
end

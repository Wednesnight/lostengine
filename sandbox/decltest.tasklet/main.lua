require("lost.declarative.Context")

hasWindow = true
windowParams = lost.application.WindowParams("testing", lost.math.Rect(200,200,600,128))

function startup(tasklet)
  dcl = lost.declarative.Context(tasklet.loader)

  rootNode = require("scene")

  return true
end

function update(tasklet)
  rootNode:process(tasklet.window.context)
  tasklet.window.context:swapBuffers()
  return true
end

function shutdown(tasklet)
  return true
end
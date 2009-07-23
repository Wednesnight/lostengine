require("lost.declarative.Context")

hasWindow = true
windowParams = lost.application.WindowParams("Ivy", lost.math.Rect(200,200,640,480))

function startup(tasklet)
  dcl = lost.declarative.Context(tasklet.loader)
  tasklet.eventDispatcher:addEventListener(lost.application.DropEvent.DROPPED_FILE, dropHandler)
  rootNode = dcl.rg:Node
  {
    name = "rootNode",
    dcl.rg:ClearColor{color=lost.common.Color(1,1,1,1)},
    dcl.rg:Clear{mask=gl.GL_COLOR_BUFFER_BIT+gl.GL_DEPTH_BUFFER_BIT},
    dcl.rg:DepthTest{false}
  }
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

function dropHandler(event)
  local dropEvent = lost.application.DropEvent.cast(event)
  local path = dropEvent.filename
  log.debug("dropped file: '"..path.."'")
end

require("lost.declarative.Context")

local MatrixTranslation = lost.math.MatrixTranslation
local Vec3 = lost.math.Vec3
local Rect = lost.math.Rect

windowParams = lost.application.WindowParams("Ivy", lost.math.Rect(200,200,640,480))

rootNode = nil
currentPicNode = nil -- receives a quad that draws the dropped bitmap at 0,0
-- create a custom loader that takes absolute filepaths 
bitmapLoader = lost.resource.Loader.create()
bitmapLoader:addRepository(lost.resource.FilesystemRepository.create("/"))

function startup()
  dcl = lost.declarative.Context(tasklet.loader)
  tasklet.eventDispatcher:addEventListener(lost.application.DragNDropEvent.DROP, dropHandler)
  rootNode = dcl.rg:Node
  {
    name = "rootNode",
    dcl.rg:ClearColor{color=lost.common.Color(1,1,1,1)},
    dcl.rg:Clear{mask=gl.GL_COLOR_BUFFER_BIT+gl.GL_DEPTH_BUFFER_BIT},
    dcl.rg:Camera2D
    {
      name = "2D Cam",
      viewport = Rect(0,0,windowParams.rect.width,windowParams.rect.height)
    },
    dcl.rg:DepthTest{false}
  }
  tasklet.waitForEvents = true
  return true
end

function update()
  rootNode:process(tasklet.window.context)
  tasklet.window.context:swapBuffers()
  return true
end

function shutdown()
  return true
end

function dropHandler(event)
  local dropEvent = lost.application.DragNDropEvent.cast(event)
  local path = dropEvent.filename
  log.debug("dropped file: '"..path.."'")
  local data = bitmapLoader:load(path)
  log.debug("loaded data of size: "..data.size)
  rootNode:remove(currentPicNode)
  local meshQuad = lost.mesh.Quad2D.create(data, true)
  currentPicNode = lost.rg.Draw.create(meshQuad)
  rootNode:add(currentPicNode)
end

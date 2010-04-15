using "lost.resource.Loader"
using "lost.resource.FilesystemRepository"
using "lost.resource.ApplicationResourceRepository"
using "lost.application.K_ESCAPE"
using "lost.application.K_SPACE"
using "lost.math.Vec2"
using "lost.math.MatrixRotX"
using "lost.math.MatrixRotY"

local modelLoader = Loader.create()
modelLoader:addRepository(FilesystemRepository.create("/"))
modelLoader:addRepository(ApplicationResourceRepository.create())

local controller =
{
  angle = Vec2(0,0),
  running = false
}

controller.keyHandler = function(event)
  if event.key == K_ESCAPE then
    controller.running = false
  elseif event.key == K_SPACE then
    if controller.model.mesh.drawMode == gl.GL_TRIANGLES then
      controller.model.mesh.drawMode = gl.GL_LINE_LOOP
    elseif controller.model.mesh.drawMode == gl.GL_LINE_LOOP then
      controller.model.mesh.drawMode = gl.GL_POINTS
    else
      controller.model.mesh.drawMode = gl.GL_TRIANGLES
    end
    controller.renderView:needsRedraw()
  elseif event.character == "+" then
    controller.sphereSubdivisions = controller.sphereSubdivisions + 1
    controller.model.mesh:updateSubdivisions(controller.sphereSubdivisions)
    controller.renderView:needsRedraw()
  elseif event.character == "-" and controller.sphereSubdivisions > 12 then
    controller.sphereSubdivisions = controller.sphereSubdivisions - 1
    controller.model.mesh:updateSubdivisions(controller.sphereSubdivisions)
    controller.renderView:needsRedraw()
  end
end

controller.droppedModel = function(event)
  if event.currentTarget == event.target then

    -- only accept .obj/.md5mesh extensions
    controller.modelFilename = string.match(event.filename, "(.*%.obj)$")

    if controller.modelFilename ~= nil then

      if controller.model == nil then
        controller.model = dcl.rg:Draw
        {
          name = "model",
          mesh = dcl.mesh:Obj
          {
            file = modelLoader:load(controller.modelFilename),
            material =
            {
              shader = dcl.gl:Shader
              {
                filename = "light"
              }
            }
          }
        }
        local rg = event.target:renderGraph()
        if rg ~= nil then
          rg:add(controller.model)
        end
        event.target:needsRedraw()
      else
        controller.model.mesh = dcl.mesh:Obj
        {
          file = modelLoader:load(controller.modelFilename),
          material =
          {
            shader = dcl.gl:Shader
            {
              filename = "light"
            }
          }
        }
        event.target:needsRedraw()
      end
    end
  end
end

controller.sceneMouseDown = function(event)
  controller.moving = true
  controller.startPos = event.pos
end

controller.sceneMouseMove = function(event)
  if controller.moving and event.currentTarget == event.target then
    if controller.model ~= nil then
      local deltaX = controller.startPos.x - event.pos.x
      local deltaY = controller.startPos.y - event.pos.y
      controller.startPos = event.pos

      controller.angle.x = math.fmod(-deltaY + controller.angle.x, 360)
      controller.angle.y = math.fmod(deltaX + controller.angle.y, 360)

      controller.model.mesh.transform = MatrixRotX(controller.angle.x) * MatrixRotY(controller.angle.y)
      event.target:needsRedraw()
    end
  end
end

controller.sceneMouseUp = function(event)
  controller.moving = false
end


-- this must be the last line!
return controller

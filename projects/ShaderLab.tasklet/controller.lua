using "lost.resource.Loader"
using "lost.resource.FilesystemRepository"
using "lost.resource.ApplicationResourceRepository"
using "lost.application.K_ESCAPE"
using "lost.guiro.Bounds"
using "lost.guiro.xabs"
using "lost.guiro.yabs"
using "lost.guiro.xrel"
using "lost.guiro.yrel"
using "lost.guiro.xleft"
using "lost.guiro.ytop"
using "lost.guiro.xright"
using "lost.guiro.ybottom"
using "lost.guiro.wrel"
using "lost.guiro.hrel"
using "lost.guiro.wabs"
using "lost.guiro.habs"
using "lost.guiro.wfit"
using "lost.guiro.hfit"
using "lost.math.Vec2"
using "lost.math.Vec3"
using "lost.math.Rect"

local shaderLoader = Loader.create()
shaderLoader:addRepository(FilesystemRepository.create("/"))
shaderLoader:addRepository(ApplicationResourceRepository.create())

local controller =
{
  angle = Vec2(0,0),
  running = false
}

controller.keyHandler = function(event)
  if event.key == K_ESCAPE then
    controller.running = false
  end
end

controller.droppedShader = function(event)
  if event.currentTarget == event.target then

    -- only accept vertex/fragment shader extensions
    local relativeName = string.match(event.filename, "([^/%.]*)%.vs$") or string.match(event.filename, "([^/%.]*)%.fs$")
    controller.shaderFilename = string.match(event.filename, "(.*)%.vs$") or string.match(event.filename, "(.*)%.fs$")

    if relativeName ~= nil and controller.shaderFilename ~= nil then

      event.target:text(relativeName)

      local shaderProgram = lost.gl.loadShader(shaderLoader, controller.shaderFilename)

      local typeName = function(paramType)
        if paramType == lost.gl.ShaderProgram.Parameter.ATTRIBUTE then
          return "attribute"
        elseif paramType == lost.gl.ShaderProgram.Parameter.UNIFORM then
          return "uniform"
        else
          return "undefined"
        end
      end

      local valueTypeName = function(paramType)
        if paramType == gl.GL_BOOL then
          return "bool"
        elseif paramType == gl.GL_BOOL_VEC2 then
          return "bvec2"
        elseif paramType == gl.GL_BOOL_VEC3 then
          return "bvec3"
        elseif paramType == gl.GL_BOOL_VEC4 then
          return "bvec4"
        elseif paramType == gl.GL_INT then
          return "int"
        elseif paramType == gl.GL_INT_VEC2 then
          return "ivec2"
        elseif paramType == gl.GL_INT_VEC3 then
          return "ivec3"
        elseif paramType == gl.GL_INT_VEC4 then
          return "ivec4"
        elseif paramType == gl.GL_FLOAT then
          return "float"
        elseif paramType == gl.GL_FLOAT_VEC2 then
          return "vec2"
        elseif paramType == gl.GL_FLOAT_VEC3 then
          return "vec3"
        elseif paramType == gl.GL_FLOAT_VEC4 then
          return "vec4"
        elseif paramType == gl.GL_FLOAT_MAT2 then
          return "mat2x2"
        elseif paramType == gl.GL_FLOAT_MAT2x3 then
          return "mat2x3"
        elseif paramType == gl.GL_FLOAT_MAT2x4 then
          return "mat2x4"
        elseif paramType == gl.GL_FLOAT_MAT3 then
          return "mat3x3"
        elseif paramType == gl.GL_FLOAT_MAT3x2 then
          return "mat3x2"
        elseif paramType == gl.GL_FLOAT_MAT3x4 then
          return "mat3x4"
        elseif paramType == gl.GL_FLOAT_MAT4 then
          return "mat4x4"
        elseif paramType == gl.GL_FLOAT_MAT4x2 then
          return "mat4x2"
        elseif paramType == gl.GL_FLOAT_MAT4x3 then
          return "mat4x3"
        elseif paramType == gl.GL_SAMPLER_1D then
          return "sampler1D"
        elseif paramType == gl.GL_SAMPLER_2D then
          return "sampler2D"
        elseif paramType == gl.GL_SAMPLER_3D then
          return "sampler3D"
        else
          return "undefined"
        end
      end

      -- clear old params
      event.target.parent("shaderParams"):removeAllSubviews()
      
      -- read shader params
      shaderProgram:enable()

      -- sort by param name
      local paramMap = shaderProgram:parameterMap()
      local paramNames = {}
      for k in next,paramMap do
        table.insert(paramNames, k)
      end
      table.sort(paramNames)

      for i,k in next,paramNames do

        local v = paramMap[k]
        local readonly = string.find(k, "^gl_") ~= nil

        local editor = nil
        if readonly then
          editor = dcl.guiro:Label
          {
            bounds = Bounds(xabs(0), yabs(0), wrel(.12), hrel(1)),
            fontSize = 10,
            text = "(builtin)",
            halign = "left"
          }
        else
          local type = valueTypeName(v.glType) or ""
          if type:find("^sampler") or type == "int" or type == "float" then
            editor = dcl.guiro:SpinEdit
            {
              bounds = Bounds(xabs(0), yabs(0), wrel(.12), hrel(1)),
              value = 0,
              min = 0,
              max = 15,
              stepSize = 1,
              listeners =
              {
                valueChanged = function(event)
                  if event.currentTarget == event.target then
                    shaderProgram:enable()
                    if type == "float" then
                      shaderProgram:setFloat(k, event.value)
                    else
                      shaderProgram:setInt(k, event.value)
                    end
                    shaderProgram:disable()
                  end
                end
              }
            }
          else
            editor = dcl.guiro:Label
            {
              bounds = Bounds(xabs(0), yabs(0), wrel(.12), hrel(1)),
              fontSize = 10,
              text = "",
              halign = "left"
            }
          end
        end

        local param = dcl.guiro:HBox
        {
          bounds = Bounds(xabs(0), yabs(0), wrel(1), habs(20)),
          showFrame = true,
          valign = "center",
          mode = "stack",
          dcl.guiro:Label
          {
            bounds = Bounds(xabs(0), yabs(0), wrel(.19), hrel(1)),
            fontSize = 10,
            text = typeName(v.paramType),
            halign = "left"
          },
          dcl.guiro:Label
          {
            bounds = Bounds(xabs(0), yabs(0), wrel(.19), hrel(1)),
            fontSize = 10,
            text = valueTypeName(v.glType),
            halign = "left"
          },
          dcl.guiro:Label
          {
            bounds = Bounds(xabs(0), yabs(0), wrel(.50), hrel(1)),
            fontSize = 10,
            text = k,
            halign = "left"
          },
          editor
        }
        event.target.parent("shaderParams"):addSubview(param)
      end
      shaderProgram:disable()
      event.target.parent.parent.parent("renderView")("scene"):shader(shaderProgram)
    end
  end
end

controller.sceneMouseDown = function(event)
  controller.moving = true
  controller.startPos = event.pos
end

controller.sceneMouseMove = function(event)
  if controller.moving and event.currentTarget == event.target then
    local mesh = event.target:renderGraph():recursiveFindByName("mesh")
    if mesh ~= nil then
      local deltaX = controller.startPos.x - event.pos.x
      local deltaY = controller.startPos.y - event.pos.y
      controller.startPos = event.pos

      controller.angle.x = math.fmod(-deltaY + controller.angle.x, 360)
      controller.angle.y = math.fmod(deltaX + controller.angle.y, 360)

      mesh.mesh.transform = lost.math.MatrixRotX(controller.angle.x) * lost.math.MatrixRotY(controller.angle.y)
      event.target:needsRedraw()
    end
  end
end

controller.sceneMouseUp = function(event)
  controller.moving = false
end

controller.reloadShader = function(event)
  if event.currentTarget == event.target then
  --
  end
end


-- this must be the last line!
return controller

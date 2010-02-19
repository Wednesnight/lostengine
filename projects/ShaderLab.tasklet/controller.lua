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

local shaderLoader = Loader.create()
shaderLoader:addRepository(FilesystemRepository.create("/"))
shaderLoader:addRepository(ApplicationResourceRepository.create())

return
{
  droppedShader = function(event)
    if event.currentTarget == event.target then
      local relativeName = string.match(event.filename, "([^/%.]*)%.[^/%.]*$")
      local filePath = string.match(event.filename, "(.*)%.[^/%.]*$")
      if relativeName ~= nil and filePath ~= nil then

        event.target:text(relativeName)

        local shaderProgram = lost.gl.loadShader(shaderLoader, filePath)

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

        event.target.parent("shaderParams"):removeAllSubviews()
        event.target.parent("shaderParams"):addSubview(dcl.guiro:Label
        {
          bounds = Bounds(xabs(0), yabs(0), wrel(1), habs(20)),
          fontSize = 12,
          text = "Parameters",
          halign = "left"
        })
        shaderProgram:enable()
        for k,v in next,shaderProgram:parameterMap() do
          local param = dcl.guiro:HBox
          {
            bounds = Bounds(xabs(0), yabs(0), wrel(1), habs(10)),
            showFrame = true,
            valign = "center",
            mode = "stack",
            dcl.guiro:Label
            {
              bounds = Bounds(xabs(0), yabs(0), wrel(.22), habs(10)),
              fontSize = 10,
              text = typeName(v.paramType),
              halign = "left"
            },
            dcl.guiro:Label
            {
              bounds = Bounds(xabs(0), yabs(0), wrel(.22), habs(10)),
              fontSize = 10,
              text = valueTypeName(v.glType),
              halign = "left"
            },
            dcl.guiro:Label
            {
              bounds = Bounds(xabs(0), yabs(0), wrel(.56), habs(10)),
              fontSize = 10,
              text = k,
              halign = "left"
            }
          }
          event.target.parent("shaderParams"):addSubview(param)
        end
        shaderProgram:disable()
      end
    end
  end
}

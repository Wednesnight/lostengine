-- lost.guiro.RenderView
module("lost.guiro", package.seeall)

require("lost.guiro.Image")

lost.common.Class "lost.guiro.RenderView" "lost.guiro.Image" {}

using "lost.application.currentTasklet"
using "lost.math.Vec2"
using "lost.math.Vec3"
using "lost.math.Rect"
using "lost.math.MatrixTranslation"
using "lost.common.Color"
using "lost.rg.DefaultFrameBuffer"
using "lost.rg.FrameBuffer"
using "lost.rg.Node"

--[[
    A RenderView manages a framebuffer that can be used to draw custom scenes within Guiro UIs
]]
function RenderView:constructor(textureManager)
  lost.guiro.Image.constructor(self, textureManager)
  -- suppress frame by default
  self:showFrame(false)
  -- suppress background by default
  self:showBackground(false)

  self._framebuffer = lost.gl.FrameBuffer.create(Vec2(16,16), gl.GL_RGBA8, gl.GL_DEPTH_COMPONENT24, -1)
  self:texture(self._framebuffer:colorTexture(0))

  self._renderViewNode = Node.create()
  self._renderViewNode.name = "renderViewNode"

  self._framebufferNode = lost.rg.FrameBuffer.create(self._framebuffer)
  self._framebufferNode.name = "framebufferNode"

  self._renderGraphNode = Node.create()
  self._renderGraphNode.name = "renderGraphNode"
  
  self._defaultFramebufferNode = lost.rg.DefaultFrameBuffer.create()
  self._defaultFramebufferNode.name = "defaultFramebufferNode"
  
  self._renderViewNode:add(self._framebufferNode)
  self._renderViewNode:add(self._renderGraphNode)
  self._renderViewNode:add(self._defaultFramebufferNode)
  -- make sure depth test is disabled
  self._renderViewNode:add(lost.rg.DepthTest.create(false))
end

function RenderView:beforeLayout()
  self._oldRect = self.rect
  lost.guiro.Image.beforeLayout(self)
end

function RenderView:afterLayout()
  -- update framebuffer on resize, check greater zero to filter setup calls
  if self._oldRect ~= self.rect and self.rect.width > 0 and self.rect.height > 0 then
    self._framebuffer:resize(Vec2(self.rect.width, self.rect.height))
    local camera = self._renderGraphNode:recursiveFindByName("camera")
    if camera ~= nil then
      camera.cam:viewport(Rect(0, 0, self.rect.width, self.rect.height))
    end
    local shader = self:shader()
    if shader ~= nil then
      shader:enable()
      for k,v in next,shader:uniformMap() do
        -- FIXME: some hard-coded values until we have edit controls
        if v.name == "width" then
          shader:setFloat(v.name, self.rect.width)
        elseif v.name == "height" then
          shader:setFloat(v.name, self.rect.height)
        elseif v.name == "screensize" then
          shader:setFloat(v.name, Vec2(self.rect.width, self.rect.height))
        end
      end
      shader:disable()
    end
    self:needsRedraw()
  end
  lost.guiro.Image.afterLayout(self)
end

function RenderView:beforeRedraw()
  self._renderViewNode:process(currentTasklet.window.context)
  self._oldShader = self:shader()
  lost.guiro.Image.beforeRedraw(self)
end

function RenderView:afterRedraw()
  lost.guiro.Image.afterRedraw(self)
  if self._oldShader ~= nil then
    if self._textureMesh ~= nil and not rawequal(self._oldShader, self._textureMesh.material.shader) then
      self._textureMesh.material.shader = self._oldShader
    end
    self._oldShader = nil
  end
end

function RenderView:renderGraph()
  return self._renderGraphNode
end

function RenderView:shader(s)
  if s ~= nil and self._textureMesh ~= nil then
    s:enable()
    for k,v in next,s:uniformMap() do
      -- FIXME: some hard-coded values until we have edit controls
      if v.name == "tex" or v.name == "colorMap" then
        s:setInt(v.name, 0)
      elseif v.name == "width" then
        s:setFloat(v.name, self.rect.width)
      elseif v.name == "height" then
        s:setFloat(v.name, self.rect.height)
      elseif v.name == "screensize" then
        s:set(v.name, Vec2(self.rect.width, self.rect.height))
      end
    end
    s:disable()

    self._textureMesh.material.shader = s
    self:needsRedraw()
  elseif self._textureMesh ~= nil then
    return self._textureMesh.material.shader
  else
    return nil
  end
end

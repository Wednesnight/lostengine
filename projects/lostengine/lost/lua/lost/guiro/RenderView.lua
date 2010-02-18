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
function RenderView:constructor()
  lost.guiro.Image.constructor(self)

  -- suppress frame by default
  self:showFrame(false)
  -- suppress background by default
  self:showBackground(false)

  self._framebuffer = lost.gl.FrameBuffer.create(Vec2(1, 1), gl.GL_RGBA8, gl.GL_DEPTH_COMPONENT24, -1)
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
    self:needsRedraw()
  end
  lost.guiro.Image.afterLayout(self)
end

function RenderView:beforeRedraw()
  self._renderViewNode:process(currentTasklet.window.context)
  lost.guiro.Image.beforeRedraw(self)
end

function RenderView:renderGraph()
  return self._renderGraphNode
end

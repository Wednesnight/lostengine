module("lost.guiro", package.seeall)

require("lost.guiro.View")
require("lost.guiro.ReceivesEvents")
--require("lost.guiro.MouseManager")


--[[
     Screen
     Holds only UserInterfaces.
     Sets up basic 2D camera and other flags for rendering.
     
     The Screen is the only view who's global rect is controlled from the outside. 
     A Screen should always span the whole physical window area OR the area of the
     physical window that you want to have covered by the guiro user interfaces.
     Therefore, you need to attach a listener to the tasklets EventDispatcher and 
     listen for resize events. If the physical window resizes, you need to tell the
     Screen the new size, so it can track it and force an update of the subviews
     if required.
  ]]
lost.common.Class "lost.guiro.Screen" "lost.guiro.View" "lost.guiro.ReceivesEvents" {}

using "lost.application.currentTasklet"

function Screen:constructor(textureManager)
  lost.guiro.View.constructor(self, textureManager)
  lost.guiro.ReceivesEvents.constructor(self)

  -- init size
  if currentTasklet ~= nil then
    local windowRect = currentTasklet.window.params.rect
    self.bounds = lost.guiro.Bounds(xabs(0), yabs(0), wabs(windowRect.width), habs(windowRect.height))
  end

  self.focusable = true
	self.parent = nil

--  self.mouseManager = lost.guiro.MouseManager(self)

  -- disable scissoring when drawing is done
  local disableScissorNode = lost.rg.Scissor.create(false)
  disableScissorNode.name = "disableScissorNode"
  self.rootNode:add(disableScissorNode)

  self.camera = lost.camera.Camera2D.create(lost.math.Rect())
--  self.renderNode:add(lost.rg.ClearColor.create(lost.common.Color(0,0,0,1)))
--  self.renderNode:add(lost.rg.Clear.create(gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT))
  self.renderNode:add(lost.rg.Camera.create(self.camera))
  self.renderNode:add(lost.rg.DepthTest.create(false))
  self:showBackground(false)
  self:showFrame(false)
  
  if currentTasklet ~= nil then
    currentTasklet.uiNode:add(self.rootNode)
  end

  -- trigger updates
  self:needsLayout()
  self:needsRedraw()
end

function Screen:setEventDispatcher(eventDispatcher)
  if eventDispatcher ~= nil then
    lost.guiro.ReceivesEvents.setEventDispatcher(self, eventDispatcher)

    eventDispatcher:addEventListener(lost.application.ResizeEvent.MAIN_WINDOW_RESIZE, function(event) 
          self.bounds.width = lost.guiro.wabs(event.width)
          self.bounds.height = lost.guiro.habs(event.height)
          self:needsLayout()
        end)
  end
end

--[[
    every time the screens layout is updated we MUST reset the viewport 
  ]]
function Screen:afterLayout()
  self.camera:viewport(self.rect)
end

--[[
    only accepts lost.guiro.UserInterface
    will fail if subview is not a lost.common.Class
  ]]
function Screen:addSubview(subview)
  if subview:isDerivedFrom("lost.guiro.UserInterface") then
    lost.guiro.HasSubviews.addSubview(self, subview)
  else
    error("Screen:addSubview() can only add subviews of type UserInterface : ".. subview:className(), 2)
  end
end

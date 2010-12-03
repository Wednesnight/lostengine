module("lost.guiro.view", package.seeall)

require("lost.common.Class")
require("lost.common.Shaders")
require("lost.guiro.event.Event")
require("lost.guiro.event.EventDispatcher")
require("lost.common.CallLater")
require("lost.guiro.layer.Layer")

using "lost.guiro.event.Event"
using "lost.guiro.event.EventDispatcher"

lost.common.Class "lost.guiro.view.View" {}
View.indices = {} -- class member for global view ids
using "lost.common.callLater"
using "lost.math.Vec2"
using "lost.math.Vec3"
using "lost.math.MatrixTranslation"

function View:constructor()
	self.layer = lost.guiro.layer.Layer()

  self.subviews = {}
  -- setup event dispatchers
  self.captureEventDispatcher = EventDispatcher()
  self.targetEventDispatcher = EventDispatcher()
  self.bubbleEventDispatcher = EventDispatcher()

  -- layout
  self.bounds = lost.guiro.Bounds(0,0,0,0)
  self.rect = lost.math.Rect()
  self.dirtyLayout = false

  -- all views and derived classes receive a default id in the
  -- constructor so they can be uniquely identified
  self.id = self:createDefaultId()

  -- RenderGraph node
  self.rootNode = lost.rg.Node.create()
  self.rootNode.name = self.id
  self.renderNode = lost.rg.Node.create()
  self.renderNode.name = "renderNode"
  self.subviewNodes = lost.rg.Node.create()
  self.subviewNodes.name = "subviewNodes"

  -- draw the view
  self.rootNode:add(self.renderNode)
  -- draw subviews
  self.rootNode:add(self.subviewNodes)

  -- meshes and draw nodes
  self.backgroundMesh = lost.mesh.RoundedRect.create(lost.guiro.textureManager()._textureManager, 
                                                     Vec2(self.rect.width, self.rect.height),
                                                     true,
                                                     8,
                                                     1)
  self.backgroundMesh:roundCorners(false, false, false, false)
  self.backgroundMesh.material.shader = lost.guiro.shaderFactory():texture()
  self.backgroundMesh.material.blend = true  
  self.backgroundMesh.material.color = lost.common.Color(1,0,0,1)
  self.backgroundNode = lost.rg.Draw.create(self.backgroundMesh)
  self.backgroundNode.name = "drawViewBackground"
  self.backgroundNode.active = false

  self.frameMesh = lost.mesh.RoundedRect.create(lost.guiro.textureManager()._textureManager,
                                                Vec2(self.rect.width, self.rect.height),
                                                false,
                                                8,
                                                1)
  self.frameMesh:roundCorners(false, false, false, false)                                                
  self.frameMesh.material.shader = lost.guiro.shaderFactory():texture()
  self.frameMesh.material.blend = true  
  self.frameMesh.material.color = lost.common.Color(1,1,1,1)
  self.frameNode = lost.rg.Draw.create(self.frameMesh)
  self.frameNode.name = "drawViewFrame"
  self.frameNode.active = false

  self.renderNode:add(self.backgroundNode)
  self.renderNode:add(self.frameNode)

  self.dirty = false
  
  self.focusable = true
end

function View:onAttach(parent)
  self:setParent(parent)
end

function View:onDetach(parent)
  self:setParent(nil)
end

--[[
    Re-/set parent view
  ]]
function View:setParent(parent)
  if parent ~= nil then
    self.parent = parent
    self.parent.subviewNodes:add(self.rootNode)
    self:needsLayout()
    self:needsRedraw()
  else
    if self.parent ~= nil then
      self.parent.subviewNodes:remove(self.rootNode)
    end
    self.parent = parent
  end
end

--[[
    Get the root view of the view hierarchy
  ]]
function View:rootView()
  if self.parent ~= nil then
    return self.parent:rootView()
  else
    return self
  end
end

--[[ 
    prints self.subviews hierarchy
  ]]
function View:printSubviews(prefix)
  if not prefix then
    prefix = ""
  end
  log.debug(prefix .."|-- ".. self.id)
  for k,view in next,self.subviews do
    view:printSubviews(prefix .."    ")
  end
end

--[[
    Utility method for auto-generated ids
  ]]
function View:createDefaultId()
  local result = nil
  local name = self:className()
  if (not View.indices[name]) then
    View.indices[name] = 1
  else
    View.indices[name] = View.indices[name]+1
  end
  result = name .. View.indices[name]
  return result
end

--[[
    Updates dirty flag and inserts self into redraw queue
  ]]
function View:needsRedraw()
  if not self.dirty then
    self.dirty = true
    
    -- add to tasklet queue
    callLater(View._redraw, self)
  end
end

-- Internal redraw, do not use! See also: View:beforeRedraw(), View:afterRedraw()
function View:_redraw()
--  log.debug(tostring(self) .."(".. self.id .."):_redraw()")
  self:beforeRedraw()
  self.dirty = false
  for k,view in next,self.subviews do
    view:_redraw(true)
  end
  self:afterRedraw()
end

-- Override this method to implement your redraw code before base objects are updated
function View:beforeRedraw()
end

--  Override this method to implement your redraw code after base objects were updated
function View:afterRedraw()
end

-- set bool flag or retrieve it
function View:showFrame(...)
  if arg.n > 0 then
    self.frameNode.active = arg[1]
  else
    return self.frameNode.active
  end
end

-- set color or retrieve it
function View:frameColor(...)
  if arg.n > 0 then
    self.frameMesh.material.color = arg[1]
  else
    return self.frameMesh.material.color
  end
end

function View:showBackground(...)
  if arg.n > 0 then
    self.backgroundNode.active = arg[1]
  else
    return self.backgroundNode.active
  end
end

function View:backgroundColor(...)
  if arg.n > 0 then
    self.backgroundMesh.material.color = arg[1]
  else
    return self.backgroundMesh.material.color
  end
end

function View:hidden(...)
	if arg.n > 0 then
		self.rootNode.active = not arg[1]
	else
		return not self.rootNode.active or (self.parent ~= nil and self.parent:hidden())
	end
end

function View:backgroundCornerRadius(val)
  self.backgroundMesh:radius(val)
end

function View:frameCornerRadius(val)
  self.frameMesh:radius(val)
end

function View:backgroundRoundCorners(t,b,l,r)
	self.backgroundMesh:roundCorners(t,b,l,r)
end

function View:frameRoundCorners(t,b,l,r)
	self.frameMesh:roundCorners(t,b,l,r)
end

function View:backgroundShowSides(t,b,l,r)
  self.backgroundMesh:showSides(t,b,l,r)
end

function View:frameShowSides(t,b,l,r)
  self.frameMesh:showSides(t,b,l,r)
end

function View:frameWidth(val)
	self.frameMesh:lineWidth(val)
end

function View:backgroundGradient(gradientName)
--function View:backgroundGradient(tex, coord)
  if gradientName ~= nil then
    local tex = ui.textureManager._textureManager.gradientTexture
    local coord = ui.textureManager:gradientCoord(gradientName)
--    log.debug("gradient "..gradientName.." coord: "..tostring(coord))
    if self.backgroundMesh.material.uniforms == nil then
      self.backgroundMesh.material.uniforms = lost.gl.UniformBlock.create()
    end
    self.backgroundMesh.material:setTexture(1, tex)
    self.backgroundMesh.material.uniforms:setFloat("gradientCoord", coord)
    self.backgroundMesh.material.shader = lost.common.Shaders.gradientShader()
  else
    self.backgroundMesh.material:limitTextures(1)
    self.backgroundMesh.shader = lost.common.Shaders.textureShader()
  end
end

function View:frameGradient(gradientName)
--function View:frameGradient(tex, coord)
  if gradientName ~= nil then
    local tex = ui.textureManager._textureManager.gradientTexture
    local coord = ui.textureManager:gradientCoord(gradientName)
    if self.frameMesh.material.uniforms == nil then
      self.frameMesh.material.uniforms = lost.gl.UniformBlock.create()
    end
    self.frameMesh.material:setTexture(1, tex)
    self.frameMesh.material.uniforms:setFloat("gradientCoord", coord)
    self.frameMesh.material.shader = lost.common.Shaders.gradientShader()
  else
    self.frameMesh.material:limitTextures(1)
    self.frameMesh.shader = lost.common.Shaders.textureShader()
  end
end

function View:__tostring()
  return self:className() .."(".. self.id ..")"
end

--[[ 
    Basically same functionality as EventDispatcher:addEventListener.
    If phase is set, the listener is only added to associated event listeners.
    If phase is not set, the listener is only added to the target listeners
  ]]
function View:addEventListener(which, listener, phase)
--  log.debug(tostring(self.id).. ": adding listener for ".. phase)

  -- default is target + bubble
  local capture = (phase ~= nil and phase == Event.PHASE_CAPTURE)
  local target = (phase == nil or phase == Event.PHASE_TARGET)
  local bubble = (phase == nil or phase == Event.PHASE_BUBBLE)

  if capture then 
    self.captureEventDispatcher:addEventListener(which, listener)
  end
  if target then 
    self.targetEventDispatcher:addEventListener(which, listener)
  end
  if bubble then 
    self.bubbleEventDispatcher:addEventListener(which, listener)
  end
end

--[[ 
    Basically same functionality as EventDispatcher:removeEventListener.
    If phase is set, the listener is only removed from the associated event listeners.
    If phase is not set, the listener is only removed from the target listeners
  ]]
function View:removeEventListener(which, listener, phase)
--  log.debug(tostring(self.id).. ": removing listener for ".. phase)

  -- default is target + bubble
  local capture = (phase ~= nil and phase == Event.PHASE_CAPTURE)
  local target = (phase == nil or phase == Event.PHASE_TARGET)
  local bubble = (phase == nil or phase == Event.PHASE_BUBBLE)

  if capture then 
    self.captureEventDispatcher:removeEventListener(which, listener)
  end
  if target then 
    self.targetEventDispatcher:removeEventListener(which, listener)
  end
  if bubble then 
    self.bubbleEventDispatcher:removeEventListener(which, listener)
  end
end

--[[
    Calling dispatchEvent on a view notifies all target listeners and bubbles the event
    if event.bubbles is enabled.
    
    If phase is set, dispatchEvent will only notify the appropriate listeners
  ]]
function View:dispatchEvent(event, phase)
  -- accept valid events only
  if event ~= nil and not self:hidden() then
    -- default is target + bubble
    local capture = (phase ~= nil and phase == Event.PHASE_CAPTURE)
    local target = (phase == nil or phase == Event.PHASE_TARGET)
    local bubble = (phase == nil or phase == Event.PHASE_BUBBLE)

    if capture then
      -- target == self since self dispatches the event
      event.target = self
      -- build capture hierarchy
      local stack = {}
      local current = self.parent
      while current ~= nil do
        table.insert(stack, 1, current)
        current = current.parent
      end
      -- dispatch capture
      for index, view in next,stack do
        event.currentTarget = view
        self.captureEventDispatcher:dispatchEvent(event)
        if event.stopDispatch then
          break
        end
      end
    end

    if target and not event.stopDispatch then
      event.currentTarget = self
      self.targetEventDispatcher:dispatchEvent(event)
    end

    if event.bubbles and not event.stopPropagation then
      local currentTarget = self.parent
      while currentTarget ~= nil and not event.stopPropagation do
        event.currentTarget = currentTarget
        currentTarget.bubbleEventDispatcher:dispatchEvent(event)
        currentTarget = currentTarget.parent
      end
    end
  end
end

--[[
    Routes the event to the appropriate EventDispatcher
  ]]
function View:routeEvent(event)
  if event ~= nil and not self:hidden() then
    if event.phase == Event.PHASE_CAPTURE then
      self.captureEventDispatcher:dispatchEvent(event)
    elseif event.phase == Event.PHASE_TARGET then
      self.targetEventDispatcher:dispatchEvent(event)
    elseif event.phase == Event.PHASE_BUBBLE then
      self.bubbleEventDispatcher:dispatchEvent(event)
    end
  end
end

--[[
    resize and call needsLayout
  ]]
function View:resize(bounds)
  self.bounds = bounds
  self:needsLayout()
end

--[[
    Updates dirtyLayout flag and inserts self into layout queue
  ]]
function View:needsLayout()
  if not self.dirtyLayout then
    self.dirtyLayout = true
    
    -- add to tasklet queue
    callLater(View._layout, self)
  end
end

function View:_layout(force)
--  log.debug(tostring(self) .."(".. self.id .."):_layout()")
  if force or self.dirtyLayout then
    self:beforeLayout()

    self.dirtyLayout = false
    
    -- update rect
    local parentRect = nil
    if self.parent ~= nil then
      parentRect = self.parent.rect
    else
      parentRect = lost.math.Rect()
    end
    self.rect = self.bounds:rect(parentRect)
--    log.debug(tostring(self) ..": ".. tostring(self.rect))
    
    -- Call invisible _layout() on all subviews
    if type(self.subviews) == "table" then
      for k,view in next,self.subviews do
        view:_layout(true)
      end
    end

    self:afterLayout()
  end
end

function View:beforeLayout()
end

function View:afterLayout()
  -- update background mesh
  self.backgroundMesh:size(Vec2(self.rect.width, self.rect.height))
  self.backgroundMesh.transform = MatrixTranslation(Vec3(self.rect.x, self.rect.y, 0))

  -- update frame mesh
  self.frameMesh:size(Vec2(self.rect.width, self.rect.height))
  self.frameMesh.transform = MatrixTranslation(Vec3(self.rect.x, self.rect.y, 0))
end

function View:containsCoord(point)
  return self.rect:contains(point)
end

--[[
    Adds subview at the end of self.subviews and calls subview:onAttach(self)
  ]]
function View:addSubview(subview)
  if (type(subview)=="table") and 
     (subview.isDerivedFrom ~= nil) then
    table.insert(self.subviews, subview)
  	subview:onAttach(self)
  else
    error("View:addSubview() expected lost.guiro.view.View, got ".. type(subview), 2)
  end
end

--[[
    Removes subview from self.subviews and calls subview:onDetach(self)
  ]]
function View:removeSubview(subview)
  for k,view in next,self.subviews do
    if rawequal(view, subview) then
      self.subviews[k] = nil
      view:onDetach(self)
    end
  end
end

function View:removeAllSubviews()
  local idx = #self.subviews
  while idx > 0 do
    local view = table.remove(self.subviews, idx)
    view:onDetach(self)
    idx = idx-1
  end
end

--[[
    returns subview with given subviewId
    nil if subviewId is invalid
  ]]
function View:__call(subviewId)
  local result = nil
  for k,view in next,self.subviews do
    if (view.id == subviewId) then
      result = view
      break
    end
  end
  return result
end

--breadth-first search for subview with given id
function View:recursiveFindById(viewId)
  local result = self(viewId)
  if result == nil then
    for k,view in next,self.subviews do
      result = view:recursiveFindById(viewId)
      if result ~= nil then
        break
      end
    end
  end
  return result
end

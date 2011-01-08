module("lost.guiro.view", package.seeall)

require("lost.common.Class")
require("lost.common.Shaders")
require("lost.guiro.event.Event")
require("lost.guiro.event.EventDispatcher")
require("lost.common.CallLater")
require("lost.guiro.layer.Layer")
require("lost.guiro.ThemeManager")

using "lost.guiro.event.Event"
using "lost.guiro.event.EventDispatcher"

lost.common.Class "lost.guiro.view.View" {}

local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local Rect = lost.math.Rect
local MatrixTranslation = lost.math.MatrixTranslation

-- optional params:
-- id: the id/name of this view, can be used for debug output or to find views in the hierarchy. ids don't have to be unique
-- bounds: the bounds of this view
-- sublayers: must only contain layers, will be added to this views layers
-- subviews: must only contain views, ill be added to this view as subviews
-- listeners: event listener functions at this view level
function View:constructor(args)
	self.layer = lost.guiro.layer.Layer()
  local t = args or {}
  self.id = t.id or "view"
  self.rect = lost.math.Rect()
	self.z = 0
  self._superview = nil
  self.subviews = {}
  self.captureEventDispatcher = EventDispatcher()
  self.targetEventDispatcher = EventDispatcher()
  self.bubbleEventDispatcher = EventDispatcher()
  if t.bounds ~=nil then
    self:bounds(lost.guiro.Bounds(unpack(t.bounds)))
  else
    self:bounds(lost.guiro.Bounds(0,0,"1","1"))
  end

  if t.layout then
    self.layout = t.layout
  else
    self.layout = nil
  end

  if t.clip then self:clip(t.clip) end

  lost.guiro.themeManager():apply(self, t.theme or lost.guiro.themeManager().defaultTheme, t.style or lost.guiro.themeManager().defaultStyle, args)
  if t.sublayers then
    for _,v in ipairs(t.sublayers) do
      self.layer:addSublayer(v)
    end
  end
  if t.subviews then
    for _,v in ipairs(t.subviews) do
      self:addSubview(v)
    end
  end
  if t.listeners then
    for event,func in pairs(t.listeners) do
--      log.debug("adding listener for event type "..event)
      self:addEventListener(event, func)
    end
  end
end

function View:superRect()
  local result = Rect()
  if self._superview then
    result = self._superview.rect
  end
  return result
end

function View:bounds(...)
  if arg.n > 0 then
    self._bounds = arg[1]
    self:needsLayout()
  else
    return self._bounds
  end
end

function View:superview(...)
  if arg.n > 0 then
    local newsv = arg[1]
--    log.debug("-- "..self.id.." setting superviewr to "..tostring(newsv))
    self._superview = newsv
  else
    return self._superview
  end
  self:updateZ()    
end

function View:rootview()
  if self:superview() then
    return self:superview():rootview()
  else
    return self
  end
end

function View:updateZ()
  if self._superview then 
    self.z = self._superview.z+1
  else
    self.z = 0
  end
  self:updateSubviewZ()
end

function View:updateSubviewZ()
  for _,v in pairs(self.subviews) do
    v.z = self.z+1
    v:updateSubviewZ()
  end
end

function View:printSubviews(prefix)
  if not prefix then
    prefix = ""
  end
  log.debug(prefix .."|-- ".. self.id)
  for k,view in next,self.subviews do
    view:printSubviews(prefix .."    ")
  end
end

function View:addSubview(view)
  if view:superview() ~= nil then view:removeFromSuperview() end
  table.insert(self.subviews, view)
  self.layer:addSublayer(view.layer)
  view:superview(self)
  view:needsLayout()
end

-- shortcut for construction of view hierarchies, just pass an array of views
function View:add(views)
  for _,v in ipairs(views) do
--    log.debug(_..tostring(v))
    self:addSubview(v)
  end
end

function View:removeSubview(view)
  for k,v in pairs(self.subviews) do
    if rawequal(v,view) then
      self.subviews[k] = nil
      self.layer:removeSublayer(view.layer)
      view:superview(nil)
      break
    end
  end
end

function View:removeFromSuperview()
  if self._superview then
    self._superview:removeSubview(self)
  end
end

function View:removeAllSubviews()
  while (#self.subviews) > 0 do
    local l = self.subviews[1]
    l:removeFromSuperview()
  end
end

function View:hidden(...)
	if arg.n > 0 then
		self.layer.renderNode.active = not arg[1]
	else
		return not self.layer.renderNode.active or (self._superview ~= nil and self._superview:hidden())
	end
end

function View:needsUpdate()
  lost.guiro.updateManager():viewNeedsUpdate(self)
end

function View:needsLayout()
  lost.guiro.updateManager():viewNeedsLayout(self)
  for _,v in pairs(self.subviews) do
    v:needsLayout()
  end
end

function View:needsDisplay()
  lost.guiro.updateManager():viewNeedsDisplay(self)
end

function View:update()
--  log.debug("-- view update ("..self.z..") "..self.id)
end

function View:updateRect()
--  log.debug("updateRect "..self.id)
  local superrect = nil
  if self._superview then
    superrect = self._superview.rect
  else
--    log.debug("!! no superview")
    superrect = Rect()
  end  
--  log.debug("updating with superrect: "..tostring(superrect))
  self.rect = self._bounds:rect(superrect)
--  log.debug(tostring(self.rect))
end

function View:updateLayout()
--  log.debug("-- view update layout ("..self.z..") "..self.id)
  self:updateRect()
  self.layer._bounds.x = self._bounds.x
  self.layer._bounds.y = self._bounds.y
  self.layer._bounds.width = lost.guiro.Bounds.decodeEntry(3,self.rect.width)
  self.layer._bounds.height = lost.guiro.Bounds.decodeEntry(4,self.rect.height)
  self.layer:needsLayout()
end

function View:updateDisplay()
--  log.debug("-- view update layout ("..self.z..") "..self.id)
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
--  log.debug(event.type)
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
      local current = self._superview
      while current ~= nil do
        table.insert(stack, 1, current)
        current = current._superview
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
--      log.debug("bubbling")
      local currentTarget = self._superview
      while currentTarget ~= nil and not event.stopPropagation do
--        log.debug("currentTarget: "..currentTarget.id)
        event.currentTarget = currentTarget
        currentTarget.bubbleEventDispatcher:dispatchEvent(event)
        currentTarget = currentTarget._superview
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

function View:containsCoord(point)
  return self.rect:contains(point)
end


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

function View:clip(...)
  if arg.n >= 1 then
    self.layer:clip(arg[1])
  else
    return self.layer:clip()
  end
end

-- lost.guiro.View
module("lost.guiro", package.seeall)

require("lost.common.Class")
require("lost.common.CallLater")
require("lost.guiro.Bounds")
require("lost.guiro.event.EventDispatcher")

local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local MatrixTranslation = lost.math.MatrixTranslation

lost.common.Class "lost.guiro.View"
{

  -- helper for auto-generated view ids
  indices = {},

  isView = true,
	parent = nil,

  focusable = false,
  mouseInside = false,
  receivesEvents = true,
--  sendsEvents = true ????

  dirtyLayout = true,
  dirtySubviewLayout = false,

  -- geometry cache
  currentGlobalRect = lost.math.Rect(),

  dirty = true,
  focused = false
}

function View:createDefaultId()
  local result = nil
  local name = self:className()
  if (not self.indices[name]) then
    self.indices[name] = 1
  else
    self.indices[name] = self.indices[name]+1
  end
  result = name .. self.indices[name]
  return result
end

--[[ 
    constructor
  ]]
function View:constructor()
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

  -- scissoring, initially enabled but inactive (will not be processed)
  self.scissorBounds = lost.guiro.Bounds(lost.guiro.xleft(), lost.guiro.ytop(), lost.guiro.wrel(1), lost.guiro.hrel(1))
  self.scissorNode = lost.rg.Scissor.cast(lost.rg.Scissor.create(true))
  self.scissorNode.active = false
  self.scissorNode.name = "scissorNode"
  self.scissorRectNode = lost.rg.ScissorRect.cast(lost.rg.ScissorRect.create(lost.math.Rect()))
  self.scissorRectNode.active = false
  self.scissorRectNode.name = "scissorRectNode"
  self.disableScissorNode = lost.rg.Scissor.cast(lost.rg.Scissor.create(false))
  self.disableScissorNode.active = false
  self.disableScissorNode.name = "disableScissorNode"

  -- draw the view
  self.rootNode:add(self.renderNode)
  -- apply the views scissoring
  self.rootNode:add(self.scissorNode:asNodePtr())
  self.rootNode:add(self.scissorRectNode:asNodePtr())
  -- draw subviews
  self.rootNode:add(self.subviewNodes)
  -- disable scissoring
  self.rootNode:add(self.disableScissorNode:asNodePtr())

  -- mesh container
  self.meshes = {}

  self.bounds = Bounds(xabs(0), yabs(0), wabs(10), habs(10))
  self.subviews = {}

  -- setup event dispatchers
  self.defaultEventDispatcher = lost.guiro.event.EventDispatcher()
  self.captureEventDispatcher = lost.guiro.event.EventDispatcher()
  self.currentGlobalRect = lost.math.Rect()

  -- meshes and draw nodes
  self.backgroundMesh = lost.mesh.Quad2D.create(self.currentGlobalRect)
  self.backgroundMesh.material.color = lost.common.Color(1,0,0,1)
  self.backgroundNode = lost.rg.Draw.create(self.backgroundMesh)
  self.backgroundNode.name = "drawViewBackground"
  self.backgroundNode.active = false

  self.frameMesh = lost.mesh.Rect2D.create(self.currentGlobalRect)
  self.frameMesh.material.color = lost.common.Color(1,1,1,1)
  self.frameNode = lost.rg.Draw.create(self.frameMesh)
  self.frameNode.name = "drawViewFrame"
  self.frameNode.active = false

  self.renderNode:add(self.backgroundNode)
  self.renderNode:add(self.frameNode)
  
end

function View:showFrame(flag)
  if flag ~= nil then
    self.frameNode.active = flag
  else
    return self.frameNode.active
  end
end

function View:frameColor(col)
  if col ~= nil then
    self.frameMesh.material.color = col
  else
    return self.frameMesh.material.color
  end
end

function View:showBackground(flag)
  if flag ~= nil then
    self.backgroundNode.active = flag
  else
    return self.backgroundNode.active
  end
end

function View:backgroundColor(col)
  if col ~= nil then
    self.backgroundMesh.material.color = col
  else
    return self.backgroundMesh.material.color
  end
end

function View:__tostring()
  return "View: "..self.id
end

function View:__eq(other)
  return rawequal(self, other)
end

--[[ 
    inserts child into self.subviews 
  ]]
function View:addSubview(newview, pos)
--	log.debug("trying to add "..newview.id)
  if (pos == nil) then
    pos = table.maxn(self.subviews) + 1
  end
  if (newview.id) then
    if (self(newview.id) == nil) then
      table.insert(self.subviews, pos, newview)
      self.subviewNodes:add(newview.rootNode)
      newview:setParent(self)
    else
      log.error("subview '".. newview.id .."' already exists")
			error("subview already exists")
    end
  else
    log.error("cannot append subview without id")
  end
end

--[[ 
    removes subview from self.subviews and sets subview.parent to nil
  ]]
function View:removeSubview(subview)
  local idx = 1
  for k,view in next,self.subviews do
    if rawequal(view, subview) then
      table.remove(self.subviews, idx)
      self.subviewNodes:remove(subview.rootNode)
      subview:setParent(nil)
      break
    end
    idx = idx+1
  end
end

function View:removeAllSubviews()
  for k,v in pairs(self.subviews) do 
    self:removeSubview(v)
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

--[[ 
    sets the parent
  ]]
function View:setParent(parent)
  self.parent = parent
end

--[[ 
    basically same functionality as EventDispatcher:addEventListener.
    if capture is true, the listener is only added to the capture event listeners
    if capture is false, the listener is only added to the default listeners (target and bubble)
  ]]
function View:addEventListener(which, listener, capture)
--  log.debug(tostring(self.id)..": adding listener for "..which)
  if capture then 
    self.captureEventDispatcher:addEventListener(which, listener)
  else
    self.defaultEventDispatcher:addEventListener(which, listener)
  end
end

-- calling dispatchEvent on a view notifies all target listeners and bubbles the event if event.bubbles is enabled
function View:dispatchEvent(event)
  if event then
    if not event.stopDispatch then
      self:dispatchTargetEvent(event) 
    end
    if event.bubbles and (not event.stopPropagation) then
      self:bubbleEvent(event)
    end
  end
end

-- helper function to bubble an event up the parent view hierarchy
function View:bubbleEvent(event)
  local currentView = self
  while currentView and (not event.stopPropagation) do
    currentView:dispatchBubbleEvent(event)
    currentView = currentView.parent
  end
end

function View:removeEventListener(which, listener, capture)
  if capture then
    self.captureEventDispatcher:removeEventListener(which, listener)
  else
    self.defaultEventDispatcher:removeEventListener(which, listener)
  end
end

--[[
    checks if point is within view rect
  ]]
function View:containsCoord(point)
  local globalRect = self:globalRect()
  return globalRect:contains(point)
end

--[[
    updates own and subviews's layout
  ]]
function View:updateLayout(forceUpdate)
  -- view needs update
  if forceUpdate or self.dirtyLayout then
    self.dirtyLayout = false
    self.dirtySubviewLayout = false
    if self.parent then
--      log.debug("fetching parents globalRect")
      local pgr = self.parent:globalRect()
--      log.debug("pgr "..pgr.x.." "..pgr.y.." "..pgr.width.." "..pgr.height)
      self.currentGlobalRect = self.bounds:rect(pgr)
      local cgr = self.currentGlobalRect
--      log.debug(" -- "..cgr.x.." "..cgr.y.." "..cgr.width.." "..cgr.height)
      self.backgroundMesh:updateSize(Vec2(self.currentGlobalRect.width, self.currentGlobalRect.height), false)
      self.backgroundMesh.transform = MatrixTranslation(Vec3(self.currentGlobalRect.x, self.currentGlobalRect.y, 0))
      self.frameMesh:updateSize(Vec2(self.currentGlobalRect.width, self.currentGlobalRect.height))
      self.frameMesh.transform = MatrixTranslation(Vec3(self.currentGlobalRect.x, self.currentGlobalRect.y, 0))
    else
      self.currentGlobalRect = self.bounds:rect(lost.math.Rect())
    end

    -- apply layout
    self:applyLayout()

    for key,view in next,self.subviews do
      view:updateLayout(true)
    end
--    incUp()
  -- subview needs update
  elseif self.dirtySubviewLayout then
    self.dirtySubviewLayout = false
    for key,view in next,self.subviews do
      if view.dirtyLayout or view.dirtySubviewLayout then
        view:updateLayout()
      end
    end
  end  
end

--[[
    update scissoring and other post-update settings
    override this method if you want to modify stuff that needs currentGlobalRect
  ]]
function View:applyLayout()
  -- apply scissoring
  if self.scissorRect ~= nil then
    self.scissorRectNode.rect = self.scissorRect
  else
    self.scissorRectNode.rect = self.currentGlobalRect
  end
end

--[[
    returns the Views rect in absolute (screen) coordinates
  ]]
function View:globalRect()
	self:updateLayout()
  return self.currentGlobalRect
end

--[[
    sets dirtyLayout and parent's dirtySubviewLayout flags
  ]]
function View:needsLayout()
  self:needsRedraw()
  if not self.dirtyLayout then
    self.dirtyLayout = true
    if self.parent then
      self.parent:needsSubviewLayout()
    end
  end
end

--[[
    sets dirtySubviewLayout flag
  ]]
function View:needsSubviewLayout()
  if not self.dirtySubviewLayout then
    self.dirtySubviewLayout = true
    if self.parent then
      self.parent:needsSubviewLayout()
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
    sets dirty flag to trigger render
  ]]
function View:needsRedraw()
  self.dirty = true
end

function View:update()
  self:updateLayout()
  if dirty then
    self.dirty = false
  end
  for k,view in next,self.subviews do
    view:update()
  end
end

function View:screen()
  if self.parent then
    return self.parent:screen()
  else
    return nil
  end
end

local subViewPrintNum = 0
--[[ 
    prints self.subviews hierarchy
  ]]
function View:printSubviews(prefix)
  if not prefix then
    prefix = ""
  end
  log.debug(prefix .."|-- ".. self.id)
  incUp()
  for k,view in next,self.subviews do
    view:printSubviews(prefix .."    ")
  end
end

function View:hidden(val)
	if val ~= nil then
		if self.rootNode then
			self.rootNode.active = not val
		else
      log.warn("------ can't set hidden, no root node")
		end
	else
		return not self.rootNode.active
	end
end

--[[
    the following three functions exist in order to hide some View implementation
    details from the EventManager and to prettify the calling code in the EventManager.
    If the structure proves to be viable, we can remove these methods and call the
    appropriate dispatchers directly from the EventManager.
  ]]
-- !!!!!!!!! DO NOT CALL DIRECTLY, ONLY USED BY EVENTMANAGER
function View:dispatchCaptureEvent(event)
  self.captureEventDispatcher:dispatchEvent(event)
end
function View:dispatchTargetEvent(event)
--  log.debug(self.id..": targetting")
  self.defaultEventDispatcher:dispatchEvent(event)
end
-- called by EventManager, don't call directly
function View:dispatchBubbleEvent(event)
  self.defaultEventDispatcher:dispatchEvent(event)
end
-- ^^^^^^^^^ DO NOT CALL DIRECTLY, ONLY USED BY EVENTMANAGER

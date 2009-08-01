module("lost.guiro", package.seeall) -- View

require("lost.common.Class")
require("lost.guiro.Bounds")
require("lost.guiro.event.EventDispatcher")

require("lost.guiro.config")

--[[
     View class
  ]]
lost.common.Class "lost.guiro.View"
{

  -- helper for auto-generated view ids
  indices = {},

  -- view specific stuff
  isView = true,
	parent = nil,

  -- event states and flags
  focusable = false,
  hidden = false,
  mouseInside = false,
  receivesEvents = true,
--  sendsEvents = true ????

  -- layout state flags
  dirtyLayout = true,
  dirtyChildLayout = false,
  currentGlobalRect = lost.math.Rect(),
  currentLocalRect = lost.math.Rect(),

  -- render state
  dirty = true,

  -- style
  style = nil,
  
  -- focus handling (set from lost.guiro.event.EventManager)
  focused = false

}

--[[ 
    constructor
  ]]
function View:create(properties)
  -- initialize id
  local name = self:className()
  if (not self.indices[name]) then
    self.indices[name] = 1
  else
    self.indices[name] = self.indices[name]+1
  end
  self.id = name .. self.indices[name]

  -- RenderGraph node
  self.rootNode = lost.rg.Node.create()
  self.rootNode.name = self.id
  self.renderNode = lost.rg.Node.create()
  self.renderNode.name = "renderNode"
  self.childNodes = lost.rg.Node.create()
  self.childNodes.name = "childNodes"
  self.rootNode:add(self.renderNode)
  self.rootNode:add(self.childNodes)

  -- mesh container
  self.meshes = {}

  self.bounds = Bounds(xabs(0), yabs(0), wabs(0), habs(0))
  self.children = {}

  -- setup event dispatchers
  self.defaultEventDispatcher = lost.guiro.event.EventDispatcher()
  self.captureEventDispatcher = lost.guiro.event.EventDispatcher()

  self:set(properties)
end

function View:set(properties)
  -- initialize properties param
  properties = properties or {}

  -- set values
  for k,v in next,properties do

    -- apply listeners from properties param
    if k == "listeners" then
      for name,value in next,v do
        self:addEventListener(name, value)
      end

    -- apply children from properties param
    elseif type(k) == "number" and type(v) == "table" and v.isView then
      self:appendChild(v)

    else
      if not self:setProperty(k, v) then
        self[k] = v
      end
    end
  end

  self:needsLayout()
  self:needsRedraw()
end

function View:setProperty(key, value)
  return false
end

function View:setId(id)
  if self.rootNode then
    self.rootNode.name = id
  end
end

function View:__newindex(member, value)
  local handled = false

  if not rawget(self, "__newindexIgnore") and type(member) == "string" then
    local setterName = "set" .. string.upper(string.sub(member, 1, 1)) .. string.sub(member, 2)
    if type(rawget(self, setterName)) == "function" then
      rawset(self, "__newindexIgnore", true)
      handled = rawget(self, setterName)(self, value) or false
      rawset(self, "__newindexIgnore", false)
    end
  end

  if not handled then
    rawset(self, member, value)
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
	log.debug("trying to add "..newview.id)
  if (pos == nil) then
    pos = table.maxn(self.subviews) + 1
  end
  if (newview.id) then
    if (self(newview.id) == nil) then
      table.insert(self.subviews, pos, newview)
      self.childNodes:add(newview.rootNode)
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
    removes child from self.children and sets child.parent to nil
  ]]
function View:removeSubview(subview)
  local idx = 1
  for k,view in next,self.subviews do
    if rawequal(view, subview) then
      table.remove(self.subviews, idx)
      self.childNodes:remove(subview.rootNode)
      subview:setParent(nil)
      break
    end
    idx = idx+1
  end
end

--[[
    returns child with given childId
    nil if childId is invalid
  ]]
function View:__call(childId)
  local result = nil
  for k,view in next,self.children do
    if (view.id == childId) then
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
  local currentView = self.parent
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

-- called by EventManager, don't call directly
function View:dispatchCaptureEvent(event)
  self.captureEventDispatcher:dispatchEvent(event)
end

-- called by EventManager, don't call directly
function View:dispatchTargetEvent(event)
--  log.debug(self.id..": targetting")
  self.defaultEventDispatcher:dispatchEvent(event)
end

-- called by EventManager, don't call directly
function View:dispatchBubbleEvent(event)
  self.defaultEventDispatcher:dispatchEvent(event)
end

--[[
    checks if point is within view rect
  ]]
function View:containsCoord(point)
  local globalRect = self:globalRect()
  return globalRect:contains(point)
end

--[[
    updates own and children's layout
  ]]
function View:updateLayout(forceUpdate)
  -- view needs update
  if forceUpdate or self.dirtyLayout then
    self.dirtyLayout = false
    self.dirtyChildLayout = false
    if self.parent then
      self.currentGlobalRect = self.bounds:rect(self.parent:globalRect())
    else
      self.currentGlobalRect = self.bounds:rect(lost.math.Rect())
    end
    self.currentLocalRect = self.bounds:rect(lost.math.Rect())

    for name,mesh in next,self.meshes do
      mesh.modelTransform = lost.math.MatrixTranslation(lost.math.Vec3(self.currentGlobalRect.x, self.currentGlobalRect.y, 0))
    end

    for key,view in next,self.children do
      view:updateLayout(true)
    end

  -- child needs update
  elseif self.dirtyChildLayout then
    self.dirtyChildLayout = false
    for key,view in next,self.children do
      if view.dirtyLayout or view.dirtyChildLayout then
        view:updateLayout()
      end
    end
  end
  
  return lost.math.Rect(self.currentGlobalRect), lost.math.Rect(self.currentLocalRect)
end

--[[
    returns the Views rect in absolute (screen) coordinates
  ]]
function View:globalRect()
  local globalRect, localRect = self:updateLayout()
  return globalRect
end

--[[
    returns the Views rect in relative (parent) coordinates
  ]]
function View:localRect()
  local globalRect, localRect = self:updateLayout()
  return localRect
end

--[[
    sets dirtyLayout and parent's dirtyChildLayout flags
  ]]
function View:needsLayout()
  self:needsRedraw()
  if not self.dirtyLayout then
    self.dirtyLayout = true
    if self.parent then
      self.parent:needsChildLayout()
    end
  end
end

--[[
    sets dirtyChildLayout flag
  ]]
function View:needsChildLayout()
  if not self.dirtyChildLayout then
    self.dirtyChildLayout = true
    if self.parent then
      self.parent:needsChildLayout()
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

function View:initialize()
  if not self.initialized then
    self.initialized = true

    -- init style
    if type(self.style) == "table" then
      self.style.type = self.style.type or "none"
      local myRect = self:globalRect()

      log.debug("initialize: ".. self.style.type)
      local backgroundMesh = nil
      local borderMesh = nil
      if string.lower(self.style.type) == "rect" then
        backgroundMesh = lost.mesh.FilledRect2D.create(myRect)
        if self.style.borderColor ~= nil then
          borderMesh = lost.mesh.Rect2D.create(myRect)
        end

      elseif string.lower(self.style.type) == "roundrect" then
        self.style.radius = self.style.radius or math.min(math.min(myRect.width, myRect.height) / 3, 25)
        self.style.steps = self.style.steps or 25
        backgroundMesh = lost.mesh.FilledRoundedRect2D.create(myRect, self.style.radius, self.style.steps)
        if self.style.borderColor ~= nil then
          borderMesh = lost.mesh.RoundedRect2D.create(myRect, self.style.radius, self.style.steps)
        end

      elseif string.lower(self.style.type) == "circle" then
        backgroundMesh = lost.mesh.FilledCircle2D.create(math.min(myRect.width, myRect.height))
        backgroundMesh.modelTransform = lost.math.MatrixTranslation(lost.math.Vec3(myRect.x, myRect.y, 0))
        if self.style.borderColor ~= nil then
          borderMesh = lost.mesh.Circle2D.create(math.min(myRect.width, myRect.height))
        end

      elseif string.lower(self.style.type) == "ellipse" then
        backgroundMesh = lost.mesh.FilledEllipse2D.create(lost.math.Vec2(myRect.width, myRect.height))
        backgroundMesh.modelTransform = lost.math.MatrixTranslation(lost.math.Vec3(myRect.x, myRect.y, 0))
        if self.style.borderColor ~= nil then
          borderMesh = lost.mesh.Ellipse2D.create(lost.math.Vec2(myRect.width, myRect.height))
        end

      elseif string.lower(self.style.type) == "image" then
        self.style.color = self.style.color or lost.common.Color(1,1,1)
        backgroundMesh = lost.mesh.Quad2D.create(self.style.image, true)
        backgroundMesh:updateSize(lost.math.Vec2(myRect.width, myRect.height), true)
        backgroundMesh.modelTransform = lost.math.MatrixTranslation(lost.math.Vec3(myRect.x, myRect.y, 0))
        if self.style.borderColor ~= nil then
          borderMesh = lost.mesh.Rect2D.create(myRect)
        end

      end

      if backgroundMesh ~= nil then
        self.style.color = self.style.color or lost.common.Color(0,0,0)
        backgroundMesh.material.color = self.style.color
        self.meshes["background"] = backgroundMesh
      end
      if borderMesh ~= nil then
        self.style.borderColor = self.style.borderColor or lost.common.Color(0,0,0)
        borderMesh.material.color = self.style.borderColor
        self.meshes["border"] = borderMesh
      end
    end

    for name,mesh in next,self.meshes do
      self.renderNode:add(lost.rg.Draw.create(mesh))
    end

    for k,view in next,self.children do
      view:initialize()
    end
  end
end

function View:update()
  self:updateLayout()
  if dirty then
    self.dirty = false
  end

  for k,view in next,self.children do
    view:update()
  end
end


--[[ 
    helper methods
  ]]

--[[ 
    prints self.children hierarchy
  ]]
function View:printChildren(prefix)
  if not prefix then
    prefix = ""
  end
  log.debug(prefix .."|-- ".. self.id)
  for k,view in next,self.children do
    view:printChildren(prefix .."    ")
  end
end

function View:screen()
  if self.parent then
    return self.parent:screen()
  else
    return nil
  end
end

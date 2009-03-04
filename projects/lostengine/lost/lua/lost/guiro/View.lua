module("lost.guiro", package.seeall) -- View

require("lost.common.Class")
require("lost.common.Object")
require("lost.guiro.Bounds")
require("lost.guiro.event.EventDispatcher")

require("lost.guiro.config")

--[[
     View class
  ]]
View = lost.common.Class("lost.guiro.View", lost.common.Object)

View.indices = {}

--[[ 
    constructor
  ]]
function View:__init(properties)
  lost.common.Object.__init(self)

  -- initialize id
  local name = self:className()
  if (not self.indices[name]) then
    self.indices[name] = 1
  else
    self.indices[name] = self.indices[name]+1
  end
  self.id = name .. self.indices[name]

  self.bounds = Bounds(xabs(0), yabs(0), wabs(0), habs(0))
  self.children = {}
  self.isView = true
	self.parent = nil
  self.hidden = false
  self.receivesEvents = true
  self.mouseInside = false
--  self.sendsEvents = true ????

  -- layout state flags
  self.dirtyLayout = true
  self.dirtyChildLayout = false
  self.currentGlobalRect = lost.math.Rect()
  self.currentLocalRect = lost.math.Rect()

  -- render state
  self.dirty = true

  -- background color
  self.backgroundColor = nil
  
  -- background image
  self.backgroundImage = nil
  self.backgroundImageBounds = nil
  self.backgroundImageFilter = nil
  self.backgroundImageStretched = true
    
  -- border color
  self.borderColor = nil
  
  -- round corners
  self.corners =  {}
  self.corners.size = nil
  self.corners.steps = nil

  -- setup event dispatchers
  self.defaultEventDispatcher = lost.guiro.event.EventDispatcher()
  self.captureEventDispatcher = lost.guiro.event.EventDispatcher()

  -- initialize scissor state
  self.scissorState = lost.gl.State.create(lost.gl.Scissor.create(true))
  
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
    elseif type(k) == "number" and type(v) == "userdata" and v.isView then
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

function View:__tostring()
  return "View: "..self.id
end

function View:__eq(other)
  return rawequal(self, other)
end

--[[ 
    inserts child into self.children 
  ]]
function View:appendChild(child, pos)
	log.debug("trying to add "..child.id)
  if (pos == nil) then
    pos = table.maxn(self.children) + 1
  end
  if (child.id) then
    if (self(child.id) == nil) then
      table.insert(self.children, pos, child)
      child:setParent(self)
    else
      log.error("child '".. child.id .."' already exists")
			error("child already exists")
    end
  else
    log.error("cannot append child without id")
  end
end

--[[ 
    removes child from self.children and sets child.parent to nil
  ]]
function View:removeChild(child)
  local idx = 1
  for k,view in next,self.children do
    if rawequal(view, child) then
      table.remove(self.children, idx)
      child:setParent(nil)
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
  if (self.parent) then
    --
  else
    --
  end
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
    gets topmost child at given point
  ]]
function View:getViewAt(point)
  local result = nil
  if self.parent then
    result = self.parent:getViewAt(point)
  end
  if (result == nil or rawequal(result, self)) then
    local globalRect = self:globalRect()
    if globalRect:contains(point) then
      local idx = table.maxn(self.children)
      while idx > 0 do
        local childRect = self.children[idx]:globalRect()
        if childRect:contains(point) then
          result = self.children[idx]
          break
        end
        idx = idx-1
      end
    end
  end
  return result
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
    triggers render on children
  ]]
function View:render(canvas, forceRender)
  local globalRect = self:globalRect()
  if forceRender or self.dirty then
    self.dirty = false

    self.scissorState:param(lost.gl.ScissorBox.create(globalRect))
    canvas.context:pushState(self.scissorState)

    self:redraw(canvas)

    for k,view in next,self.children do
      view:render(canvas, true)
    end
    canvas.context:popState()
  end
end

--[[
    sets dirty flag to trigger render
  ]]
function View:needsRedraw()
  self.dirty = true
end

function View:redraw(canvas)
  -- draw background with image
  if self.backgroundImage then
    -- initialize background color
    if not self.backgroundColor then
      self.backgroundColor = lost.common.Color(1,1,1)
    end

    -- initialize backgroundTexture
    if not self.backgroundTexture then
      self.backgroundTextureParams = lost.gl.Texture.Params()
      self.backgroundTexture = lost.gl.Texture()
      self.backgroundTexture:init(self.backgroundImage, self.backgroundTextureParams)
    end
    self.backgroundTexture:bind()
    if self.backgroundImageFilter then
      self.backgroundTexture:filter(self.backgroundImageFilter)
    end

    -- valid image corner bounds
    if self.backgroundImageBounds then
      -- draw background image and preserve corners
      self:renderBackgroundImagePreserveCorners(canvas)
    else
      -- draw background image
      self:renderBackgroundImage(canvas)
    end
  else
    -- valid background color
    if self.backgroundColor then
      -- draw background filled
      self:renderBackgroundFilled(canvas)
    end
  end

  -- valid border color
  if self.borderColor then
    -- draw background outline
    self:renderBackgroundOutline(canvas)
  end
end

function View:renderBackgroundOutline(canvas)
  local globalRect = self:globalRect()

  canvas:setColor(self.borderColor)
  if self.corners.size then
    if self.corners.steps then
      canvas:drawRectOutlineRounded(globalRect, self.corners.size, self.corners.steps)
    else
      canvas:drawRectOutlineRounded(globalRect, self.corners.size, 25)
    end
  else
    canvas:drawRectOutline(globalRect)
  end
end

function View:renderBackgroundFilled(canvas)
  local globalRect = self:globalRect()

  canvas:setColor(self.backgroundColor)
  if self.corners.size then
    if self.corners.steps then
      canvas:drawRectFilledRounded(globalRect, self.corners.size, self.corners.steps)
    else
      canvas:drawRectFilledRounded(globalRect, self.corners.size, 25)
    end
  else
    canvas:drawRectFilled(globalRect)
  end
end

function View:renderBackgroundImage(canvas)
  local globalRect = self:globalRect()
  local imageRect = lost.math.Rect(globalRect)

  canvas:setColor(self.backgroundColor)
  if not self.backgroundImageStretched then
    local ratio = lost.math.Vec2(globalRect.width/self.backgroundImage.width, globalRect.height/self.backgroundImage.height)
    if (ratio.x <= ratio.y) then
      imageRect.width  = math.min(imageRect.width, self.backgroundImage.width * ratio.x)
      imageRect.height = math.min(imageRect.height, self.backgroundImage.height * ratio.x)
    else
      imageRect.width  = math.min(imageRect.width, self.backgroundImage.width * ratio.y)
      imageRect.height = math.min(imageRect.height, self.backgroundImage.height * ratio.y)
    end
  end
  canvas:drawRectTextured(imageRect, self.backgroundTexture, true)
end

function View:renderBackgroundImagePreserveCorners(canvas)
  local globalRect = self:globalRect()

  canvas:setColor(self.backgroundColor)
  if not self.backgroundMesh then
    self.backgroundMesh = lost.gl.Mesh2D(16, 16, 54, self.backgroundTexture)
  end

  --[[
      calculate raster:
         0  1  2  3
         4  5  6  7
         8  9 10 11
        12 13 14 15
    ]]
  local topLeft = lost.math.Rect(globalRect.x,
                                 globalRect:maxY() - self.backgroundImageBounds.top,
                                 self.backgroundImageBounds.left,
                                 self.backgroundImageBounds.top)
  local middleLeft = lost.math.Rect(globalRect.x,
                                    globalRect.y + self.backgroundImageBounds.bottom,
                                    self.backgroundImageBounds.left,
                                    (globalRect.height - 1 - self.backgroundImageBounds.top) - self.backgroundImageBounds.bottom)
  local bottomLeft = lost.math.Rect(globalRect.x,
                                    globalRect.y,
                                    self.backgroundImageBounds.left,
                                    self.backgroundImageBounds.bottom)
  local topCenter = lost.math.Rect(globalRect.x + self.backgroundImageBounds.left,
                                   globalRect:maxY() - self.backgroundImageBounds.top,
                                   (globalRect.width - 1 - self.backgroundImageBounds.left) - self.backgroundImageBounds.right,
                                   self.backgroundImageBounds.top)
  local middleCenter = lost.math.Rect(globalRect.x + self.backgroundImageBounds.left,
                                      globalRect.y + self.backgroundImageBounds.bottom,
                                      (globalRect.width - 1 - self.backgroundImageBounds.left) - self.backgroundImageBounds.right,
                                      (globalRect.height - 1 - self.backgroundImageBounds.top) - self.backgroundImageBounds.bottom)
  local bottomCenter = lost.math.Rect(globalRect.x + self.backgroundImageBounds.left,
                                      globalRect.y,
                                      (globalRect.width - 1 - self.backgroundImageBounds.left) - self.backgroundImageBounds.right,
                                      self.backgroundImageBounds.bottom)
  local topRight = lost.math.Rect(globalRect:maxX() - self.backgroundImageBounds.right,
                                  globalRect:maxY() - self.backgroundImageBounds.top,
                                  self.backgroundImageBounds.right,
                                  self.backgroundImageBounds.top)
  local middleRight = lost.math.Rect(globalRect:maxX() - self.backgroundImageBounds.right,
                                     globalRect.y + self.backgroundImageBounds.bottom,
                                     self.backgroundImageBounds.right,
                                     (globalRect.height - 1 - self.backgroundImageBounds.top) - self.backgroundImageBounds.bottom)
  local bottomRight = lost.math.Rect(globalRect:maxX() - self.backgroundImageBounds.right,
                                     globalRect.y,
                                     self.backgroundImageBounds.right,
                                     self.backgroundImageBounds.bottom)

  --[[
      set vertices
    ]]
  self.backgroundMesh:setVertex(0, lost.math.Vec2(topLeft.x - 0.5, topLeft.y + topLeft.height + 0.5))
  self.backgroundMesh:setVertex(1, lost.math.Vec2(topLeft.x + topLeft.width + 0.5, topLeft.y + topLeft.height + 0.5))
  self.backgroundMesh:setVertex(2, lost.math.Vec2(topCenter.x + topCenter.width + 0.5, topCenter.y + topCenter.height + 0.5))
  self.backgroundMesh:setVertex(3, lost.math.Vec2(topRight.x + topRight.width + 0.5, topRight.y + topRight.height + 0.5))

  self.backgroundMesh:setVertex(4, lost.math.Vec2(topLeft.x - 0.5, topLeft.y - 0.5))
  self.backgroundMesh:setVertex(5, lost.math.Vec2(topCenter.x - 0.5, topCenter.y - 0.5))
  self.backgroundMesh:setVertex(6, lost.math.Vec2(topRight.x - 0.5, topRight.y - 0.5))
  self.backgroundMesh:setVertex(7, lost.math.Vec2(topRight.x + topRight.width + 0.5, topRight.y - 0.5))

  self.backgroundMesh:setVertex(8, lost.math.Vec2(middleLeft.x - 0.5, middleLeft.y - 0.5))
  self.backgroundMesh:setVertex(9, lost.math.Vec2(middleCenter.x - 0.5, middleCenter.y - 0.5))
  self.backgroundMesh:setVertex(10, lost.math.Vec2(middleRight.x - 0.5, middleRight.y - 0.5))
  self.backgroundMesh:setVertex(11, lost.math.Vec2(middleRight.x + middleRight.width + 0.5, middleRight.y - 0.5))

  self.backgroundMesh:setVertex(12, lost.math.Vec2(bottomLeft.x - 0.5, bottomLeft.y - 0.5))
  self.backgroundMesh:setVertex(13, lost.math.Vec2(bottomCenter.x - 0.5, bottomCenter.y - 0.5))
  self.backgroundMesh:setVertex(14, lost.math.Vec2(bottomRight.x - 0.5, bottomRight.y - 0.5))
  self.backgroundMesh:setVertex(15, lost.math.Vec2(bottomRight.x + bottomRight.width + 0.5, bottomRight.y - 0.5))

  --[[
      set faces
    ]]
  local vertex = 0
  local idx = 0
  for idx = 0, 53, 18 do
    local colIdx = idx
    for col = 1, 3, 1 do
      self.backgroundMesh:setFace(colIdx, vertex) self.backgroundMesh:setFace(colIdx+1, vertex+4) self.backgroundMesh:setFace(colIdx+2, vertex+5)
      self.backgroundMesh:setFace(colIdx+3, vertex) self.backgroundMesh:setFace(colIdx+4, vertex+1) self.backgroundMesh:setFace(colIdx+5, vertex+5)
      vertex = vertex + 1
      colIdx = colIdx + 6
    end
    vertex = vertex + 1
  end

  --[[
      set texcoords
    ]]
  self.backgroundMesh:setTexcoord(0, lost.math.Vec2(0, 0))
  self.backgroundMesh:setTexcoord(1, lost.math.Vec2(topLeft.width / self.backgroundImage.width, 0))
  self.backgroundMesh:setTexcoord(2, lost.math.Vec2(1 - topRight.width / self.backgroundImage.width, 0))
  self.backgroundMesh:setTexcoord(3, lost.math.Vec2(1, 0))

  self.backgroundMesh:setTexcoord(4, lost.math.Vec2(0, self.backgroundImageBounds.top / self.backgroundImage.height))
  self.backgroundMesh:setTexcoord(5, lost.math.Vec2(topLeft.width / self.backgroundImage.width, self.backgroundImageBounds.top / self.backgroundImage.height))
  self.backgroundMesh:setTexcoord(6, lost.math.Vec2(1 - topRight.width / self.backgroundImage.width, self.backgroundImageBounds.top / self.backgroundImage.height))
  self.backgroundMesh:setTexcoord(7, lost.math.Vec2(1, self.backgroundImageBounds.top / self.backgroundImage.height))

  self.backgroundMesh:setTexcoord(8, lost.math.Vec2(0, 1 - self.backgroundImageBounds.bottom / self.backgroundImage.height))
  self.backgroundMesh:setTexcoord(9, lost.math.Vec2(topLeft.width / self.backgroundImage.width, 1 - self.backgroundImageBounds.bottom / self.backgroundImage.height))
  self.backgroundMesh:setTexcoord(10, lost.math.Vec2(1 - topRight.width / self.backgroundImage.width, 1 - self.backgroundImageBounds.bottom / self.backgroundImage.height))
  self.backgroundMesh:setTexcoord(11, lost.math.Vec2(1, 1 - self.backgroundImageBounds.bottom / self.backgroundImage.height))

  self.backgroundMesh:setTexcoord(12, lost.math.Vec2(0, 1))
  self.backgroundMesh:setTexcoord(13, lost.math.Vec2(topLeft.width / self.backgroundImage.width, 1))
  self.backgroundMesh:setTexcoord(14, lost.math.Vec2(1 - topRight.width / self.backgroundImage.width, 1))
  self.backgroundMesh:setTexcoord(15, lost.math.Vec2(1, 1))

  canvas:drawMesh2D(self.backgroundMesh, gl.GL_TRIANGLES)
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

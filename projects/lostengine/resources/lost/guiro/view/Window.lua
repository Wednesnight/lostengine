module("lost.guiro.view", package.seeall)

require("lost.guiro.view.View")
require("lost.guiro.event.Event")

local Event = lost.guiro.event.Event
local Vec2 = lost.math.Vec2

lost.common.Class "lost.guiro.view.Window" "lost.guiro.view.View" {}

-- provides windows as commonly known from operating system with GUIs.
-- The behaviour and features are defined by the components provided by the theme and some flags set
-- for a window instance.
--
-- * headerView: this view usually contains optional close buttons and a optional title. 
--   * closeButton: if prsent, the window will close if this button is clicked
--   * titleLabel: if present, the window title can be programmatically set
-- * contentView: must be present. All subviews will be added to this view. The theme can adjust the contentView size
--   so that none of the actual content will overlap window decoration.
-- * resizeButton: if present, the window will be resizable when this button is pushed and move events are registered.
-- All views can be styled via layers as usual.
function Window:constructor(args)
  self.headerView = nil -- optional, set this from theme to optinally enable movement by header dragging
  self.contentView = nil -- mandatory,must be set by theme or no subviews will be added into content area
  self.titleLabel = nil -- optional, 
  lost.guiro.view.View.constructor(self, args)
  local t = args or {}
  self.focusable = true
  self.id = t.id or "window"
  if t.movable ~= nil then
    self.movable = t.movable
  else
    self.movable = true
  end
  if t.moveType then
    self.moveType = t.moveType
  else
    if self.movable then
      if self.headerView then 
        self.moveType = "header"
      else
        self.moveType = "body"
      end
    else
    end
  end
  if t.title then self:title(t.title) end

  self.mouseDownHandler = function(event) self:mouseDown(event) end
  self.mouseUpHandler = function(event) self:mouseUp(event) end
  self.mouseMoveHandler = function(event) self:mouseMove(event) end

  self.resizeMouseDownHandler = function(event) self:resizeMouseDown(event) end
  self.resizeMouseUpHandler = function(event) self:resizeMouseUp(event) end

  if self.movable and self.moveType == "header" then self:setupMoveHeader()
  elseif self.movable and self.moveType == "body" then self:setupMoveBody() 
  end

  self.resizable = t.resizable or false
  if self.resizable then self:setupResizing() end
    
  if self.closeButton then 
    self.closeButton:addEventListener("buttonClick", function(event) self:closeButtonClick(event) end)
  end
end

function Window:setupResizing()
  self.resizeView:addEventListener("mouseDown", self.resizeMouseDownHandler)
  self.resizeView:addEventListener("mouseUp", self.resizeMouseUpHandler)
  self.resizeView:addEventListener("mouseUpOutside", self.resizeMouseUpHandler)
end

function Window:resizeMouseDown(event)
  self.resizeInProgress = true
  self.downPos = event.pos
  self.windowSize = Vec2(self.rect.width, self.rect.height)
  self.windowPos = Vec2(self.rect.x - self:superview().rect.x, self.rect.y - self:superview().rect.y)  
  self:rootview():addEventListener("mouseMove", self.mouseMoveHandler)  
end

function Window:resizeMouseUp(event)
  self.resizeInProgress = false
end

function Window:closeButtonClick(event)
  self:dispatchWindowEvent("windowClose")
  self.moveInProgress = false -- or window will be stuck in move mode upon un-hide
  self:close() -- set hidden flag AFTER dispatching event, or event won't make it
end

function Window:setupMoveHeader()
  self.headerView:addEventListener("mouseDown",self.mouseDownHandler)
  self.headerView:addEventListener("mouseUp",self.mouseUpHandler)
end

function Window:setupMoveBody()
  self:addEventListener("mouseDown",self.mouseDownHandler)
  self:addEventListener("mouseUp",self.mouseUpHandler)
end

function Window:mouseDown(event)
  if event.target ~= self.closeButton then
    self.downPos = event.pos
    self.windowPos = Vec2(self.rect.x - self:superview().rect.x, self.rect.y - self:superview().rect.y) -- window pos in local coordinates of superview
    self.moveInProgress = true
    self:rootview():addEventListener("mouseMove", self.mouseMoveHandler)
  end
end

function Window:mouseUp(event)
  self.moveInProgress = false
  self:rootview():removeEventListener("mouseMove", self.mouseMoveHandler)
end

function Window:mouseMove(event)
  if self.moveInProgress then
    local delta = event.pos - self.downPos
    local newWinPos = self.windowPos + delta
    self._bounds.x = lost.guiro.Bounds.decodeEntry(1,newWinPos.x)
    self._bounds.y = lost.guiro.Bounds.decodeEntry(2,newWinPos.y)
    self:needsLayout()
  elseif self.resizeInProgress then
    local delta = event.pos - self.downPos
    local newSize = Vec2(self.windowSize.x + delta.x, self.windowSize.y - delta.y)
    local newWinPos = self.windowPos + delta
    self._bounds.width = lost.guiro.Bounds.decodeEntry(3,newSize.x)
    self._bounds.height = lost.guiro.Bounds.decodeEntry(4,newSize.y)
    self._bounds.y = lost.guiro.Bounds.decodeEntry(2,newWinPos.y)
    self:needsLayout()
  end
end

function Window:addSubview(view,forceSubview)
  if self.contentView and not forceSubview then
    self.contentView:addSubview(view)
  else
    lost.guiro.view.View.addSubview(self, view)
  end
end

function Window:title(...)
  if arg.n >=1 then
    if self.titleLabel then
      self.titleLabel:text(arg[1])
    end
  else
    if self.titleLabel then
      return self.titleLabel:text()
    else
      return nil
    end
  end
end

function Window:dispatchWindowEvent(name)
  local event = lost.guiro.event.Event(name)
  event.bubbles = true
  event.target = self
  self:dispatchEvent(event)  
end

function Window:open()
  lost.guiro.windowManager():openWindow(self)
end

function Window:close()
  lost.guiro.windowManager():closeWindow(self)
end

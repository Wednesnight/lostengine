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
  local t = args or {}
  self.headerView = nil -- optional, set this from theme to optinally enable movement by header dragging
  self.contentView = nil -- mandatory,must be set by theme or no subviews will be added into content area
  self.titleLabel = nil -- optional, 
  lost.guiro.view.View.constructor(self, args)
  self.id = t.id or "window"
  if t.movable then
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

  if self.movable and self.moveType == "header" then self:setupMoveHeader()
  elseif self.movable and self.moveType == "body" then self:setupMoveBody() 
  end
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
--  log.debug("down: "..tostring(event.pos).." : "..tostring(event.absPos))
  self.downPos = event.pos
  self.windowPos = Vec2(self.rect.x - self:superview().rect.x, self.rect.y - self:superview().rect.y) -- window pos in local coordinates of superview
  self.moveInProgress = true
--  log.debug("winpos: "..tostring(self.windowPos))
  self:rootview():addEventListener("mouseMove", self.mouseMoveHandler)
end

function Window:mouseUp(event)
--  log.debug("up: "..tostring(event.pos).." : "..tostring(event.absPos))
  self.moveInProgress = false
  self:rootview():removeEventListener("mouseMove", self.mouseMoveHandler)
end

function Window:mouseMove(event)
--  log.debug("move: "..tostring(event.pos).." : "..tostring(event.absPos))
  if self.moveInProgress then
    local delta = event.pos - self.downPos
    local newWinPos = self.windowPos + delta
    self._bounds.x = lost.guiro.Bounds.decodeEntry(1,newWinPos.x)
    self._bounds.y = lost.guiro.Bounds.decodeEntry(2,newWinPos.y)
    self:needsLayout()
  end
end

function Window:addSubview(view)
  if self.contentView then
--    log.debug("adding "..view.id.." to contentView")
    self.contentView:addSubview(view)
  else
--    log.debug("adding "..view.id.." as window subview")
    lost.guiro.view.View.addSubview(self, view)
  end
end

function Window:title(...)
  if arg.n >=1 then
    self.titleLabel:text(arg[1])
  else
    if self.titleLabel then
      return self.titleLabel:text()
    else
      return nil
    end
  end
end

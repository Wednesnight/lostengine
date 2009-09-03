module("lost.guiro", package.seeall)

require("lost.guiro.Bounds")
require("lost.guiro.View")
require("lost.guiro.Button")
require("lost.guiro.HBox")
require("lost.guiro.VBox")
require("lost.common.CallLater")

local Bounds = lost.guiro.Bounds
local xabs = lost.guiro.xabs
local yabs = lost.guiro.yabs
local xrel = lost.guiro.xrel
local yrel = lost.guiro.yrel
local xright = lost.guiro.xright
local xleft = lost.guiro.xleft
local ytop = lost.guiro.ytop
local ybottom = lost.guiro.ybottom
local xcenter = lost.guiro.xcenter
local ycenter = lost.guiro.ycenter
local wabs = lost.guiro.wabs
local habs = lost.guiro.habs
local wrel = lost.guiro.wrel
local hrel = lost.guiro.hrel

lost.common.Class "lost.guiro.Scrollbar" "lost.guiro.View" {}

function Scrollbar:constructor()
	lost.guiro.View.constructor(self)

  self.deferredUpdateLayout = function() self:updateLayout(true) end
  self.deferredRender = function() self:render() end
  self.deferredUpdateMoveButton = function() self:updateMoveButton() end

	self._upButton = lost.guiro.Button()
	self._upButton:backgroundColor(lost.common.Color(.6,.6,.6))
	self._upButton:addEventListener("mouseDown",
	    function(event)
	      if event.target == self._upButton then
	        self:value(self._value - self._stepping)
	      end
	    end)

  self.mouseMoved = function(event)
	      if self._dragging then
          local delta = nil
          local boxRange = nil
	        if self._orientation == "horizontal" then
            delta = event.pos.x - self._dragStart.x
	          boxRange = self._moveButtonContainer:globalRect().width
	        elseif self._orientation == "vertical" then
            delta = self._dragStart.y - event.pos.y
            boxRange = self._moveButtonContainer:globalRect().height
          else
            error("invalid Scrollbar.orientation: ".. self._orientation)
	        end
          local range = math.max(1, math.abs(self._max - self._min))
          local size = math.max(0.1, math.min(0.5, boxRange/range))
          self:value(self._value + (((delta / boxRange) / (1 - size)) * range) + self._min)
          self._dragStart = event.pos
	      end
	    end

	self._moveButtonContainer = lost.guiro.View()
	self._moveButton = lost.guiro.Button()
	self._moveButton:backgroundColor(lost.common.Color(.3,.3,.3))
	self._moveButtonContainer:addSubview(self._moveButton)
	self._moveButton:addEventListener("mouseDown",
	    function(event)
	      if event.target == self._moveButton then
	        self._dragging = true
	        self._dragStart = event.pos
          self:screen():addEventListener(lost.guiro.event.MouseEvent.MOUSE_MOVE, self.mouseMoved)
	      end
	    end)
	self._moveButton:addEventListener("mouseUp",
	    function(event)
	      if event.target == self._moveButton then
	        self._dragging = false
          self:screen():removeEventListener(lost.guiro.event.MouseEvent.MOUSE_MOVE, self.mouseMoved)
	      end
	    end)
	self._moveButton:addEventListener("mouseUpOutside",
	    function(event)
	      if event.target == self._moveButton then
	        self._dragging = false
          self:screen():removeEventListener(lost.guiro.event.MouseEvent.MOUSE_MOVE, self.mouseMoved)
	      end
	    end)

	self._downButton = lost.guiro.Button()
	self._downButton:backgroundColor(lost.common.Color(.6,.6,.6))
	self._downButton:addEventListener("mouseDown",
	    function(event)
	      if event.target == self._downButton then
	        self:value(self._value + self._stepping)
	      end
	    end)

	self:orientation("horizontal") -- possible values: "horizontal", "vertical"
  self:min(0)
  self:max(100)
  self:value(0)
  self:stepping(1)
end

function Scrollbar:updateLayout(forceUpdate)
  lost.guiro.View.updateLayout(self, forceUpdate)

  local doUpdateLayout = forceUpdate or self.dirtyLayout
  if doUpdateLayout then
    --
  end
end

function Scrollbar:render()
  self:updateLayout(true)
end

function Scrollbar:orientation(orientation)
  if orientation ~= nil then
    if orientation ~= self._orientation then
      self._orientation = orientation

      if self._box ~= nil then
        self:removeSubview(self._box)
        self._box = nil
      end

      if self._orientation == "horizontal" then
      	self._downButton.bounds = Bounds(xleft(), ybottom(), wabs(15), hrel(1))
      	self._moveButtonContainer.bounds = Bounds(xleft(), ybottom(), wrel(1, -30), hrel(1))
      	self._upButton.bounds = Bounds(xleft(), ybottom(), wabs(15), hrel(1))
      	self._box = lost.guiro.HBox()
        self._box:addSubview(self._downButton)
        self._box:addSubview(self._moveButtonContainer)
        self._box:addSubview(self._upButton)
      elseif self._orientation == "vertical" then
      	self._downButton.bounds = Bounds(xleft(), ybottom(), wrel(1), habs(15))
      	self._moveButtonContainer.bounds = Bounds(xleft(), ybottom(), wrel(1), hrel(1, -30))
      	self._upButton.bounds = Bounds(xleft(), ybottom(), wrel(1), habs(15))
      	self._box = lost.guiro.VBox()
        self._box:addSubview(self._upButton)
        self._box:addSubview(self._moveButtonContainer)
        self._box:addSubview(self._downButton)
      else
        error("invalid Scrollbar.orientation: ".. self._orientation)
      end

    	self._box.bounds = Bounds(xleft(), ybottom(), wrel(1), hrel(1))
    	self._box:halign("center")
    	self._box:valign("center")
    	self._box:mode("stack")
      self:addSubview(self._box)

    end
  else
    return self._orientation
  end
end

function Scrollbar:min(min)
  if min ~= nil then
    self._min = min
    self:value(self:value())
    callLater(self.deferredUpdateMoveButton)
  else
    return self._min
  end
end

function Scrollbar:max(max)
  if max ~= nil then
    self._max = max
    self:value(self:value())
    callLater(self.deferredUpdateMoveButton)
  else
    return self._max
  end
end

function Scrollbar:value(value)
  if value ~= nil then
    self._value = math.max(self._min, math.min(self._max, value))
    callLater(self.deferredUpdateMoveButton)
  else
    return self._value
  end
end

function Scrollbar:stepping(stepping)
  if stepping ~= nil then
    self._stepping = stepping
    callLater(self.deferredUpdateMoveButton)
  else
    return self._stepping
  end
end

function Scrollbar:updateMoveButton()
  local range = math.max(1, math.abs(self._max - self._min))
  local posFactor = (self._value - self._min) / range

  if self._orientation == "horizontal" then
    local boxRange = self._moveButtonContainer:globalRect().width
    local size = math.max(0.1, math.min(0.5, boxRange/range))
    self._moveButton.bounds = Bounds(xleft({abs=((1-size)*posFactor)*boxRange}), ybottom(), wrel(size), hrel(1))
  elseif self._orientation == "vertical" then
    local boxRange = self._moveButtonContainer:globalRect().height
    local size = math.max(0.1, math.min(0.5, boxRange/range))
    self._moveButton.bounds = Bounds(xleft(), ytop({abs=-((1-size)*posFactor)*boxRange}), wrel(1), hrel(size))
  else
    error("invalid Scrollbar.orientation: ".. self._orientation)
  end
  self._moveButton:needsLayout()
end

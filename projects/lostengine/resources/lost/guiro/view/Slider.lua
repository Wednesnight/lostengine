module("lost.guiro.view", package.seeall)

require("lost.guiro.view.View")

lost.common.Class "lost.guiro.view.Slider" "lost.guiro.view.View" {}

-- Slider will modify either x or y value of handle, depending on the selected mode
-- mandatory:
-- * handleReleasedLayer (must be set by theme)
-- * handlePushedLayer (must be set by theme)
-- * handleSize (must be set by theme) this is the width for horizontal or the height for vertical
-- optional
-- * mode: "horizontal" or "vertical", defaults to "horizontal"
-- * minValue: defaults to 0
-- * maxValue: defaults to 1
--
-- FIXME: no stepping yet, no automatic stepmarks yet
--
function Slider:constructor(args)
  lost.guiro.view.View.constructor(self, args)
  local t = args or {}
  self.id = t.id or "slider"
  
  
  self.mouseDownHandler = function(event) self:mouseDown(event) end
  self.mouseUpHandler = function(event) self:mouseUp(event) end
  self:addEventListener("mouseDown", self.mouseDownHandler)
  self:addEventListener("mouseUp", self.mouseUpHandler)

  self:needsLayout()
  self:value(0)
  self.dragInProgress = false
  self:release()
end

function Slider:value(val)
end

function Slider:mouseDown(event)
  log.debug("down")
  self:press()
end

function Slider:mouseUp(event)
  log.debug("up")
  self:release()
end

function Slider:press()
  self.handleReleasedLayer:hide()
  self.handlePushedLayer:show()
end

function Slider:release()
  self.handleReleasedLayer:show()
  self.handlePushedLayer:hide()
end

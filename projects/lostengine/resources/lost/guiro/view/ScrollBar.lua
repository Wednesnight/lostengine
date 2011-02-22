module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.ScrollBar" "lost.guiro.view.View" {}

-- * handleSize: [0..1] in percent of the current grooveSize
-- * handleLayer: must be inside grooveLayer
-- * grooveLayer: background of handle, contains handle
-- * upButton
-- * downButton
--
-- controls, set by theme
-- * slider
-- * incButton
-- * decButton
--
function ScrollBar:constructor(args)
  lost.guiro.view.View.constructor(self, args)
  local t = args or {}
  self.id = t.id or "scrollbar"
  self:needsLayout()
  
  self.incButton:addEventListener("mouseDown", function(event) self:incButtonDown(event) end)
  self.incButton:addEventListener("mouseUp", function(event) self:incButtonUp(event) end)
  self.decButton:addEventListener("mouseDown", function(event) self:decButtonDown(event) end)
  self.decButton:addEventListener("mouseUp", function(event) self:decButtonUp(event) end)
end

function ScrollBar:incButtonDown(event)
  log.debug("inc "..event.type)
end

function ScrollBar:incButtonUp(event)
log.debug("inc "..event.type)
end

function ScrollBar:decButtonDown(event)
log.debug("dec "..event.type)
end

function ScrollBar:decButtonUp(event)
log.debug("dec "..event.type)
end

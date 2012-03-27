module("lost.guiro.view", package.seeall)

local Color = lost.common.Color

lost.common.Class "lost.guiro.view.ColorPicker" "lost.guiro.view.Button" {}

-- mandatory parameters set by theme
-- * colorLayer: will get it's color set to the ColorPickerWindow's current color
-- * windowTheme: theme of the ColorPickerWindow that will be opened upon click
-- * windowStyle: style of the ColorPickerWindow that will be opened upon click
-- optional:
-- * name: will be used in ColorPickerWindow title for identification
function ColorPicker:constructor(args)
  lost.guiro.view.Button.constructor(self, args)
  local t = args or {}
  self.id = t.id or "colorpicker"
  self._color = t.color or Color(1,1,1)
  self.name = t.name or ""
  self:addEventListener("buttonClick", function(event) self:buttonClick(event) end)
	self._bbcounter = lost.profiler.BBCount("lost.guiro.view.ColorPicker")        
end

function ColorPicker:buttonClick(event)
  lost.guiro.view.colorPickerWindow(self.windowTheme, self.windowStyle, self)
end

function ColorPicker:color(...)
  if arg.n >= 1 then
    self._color=arg[1]
    self.colorLayer:color(self._color)
  else
    return self._color
  end
end

-- triggered by ColorPickerWindow so color setter doesn't always broadcast it's changes
function ColorPicker:dispatchValueChangedEvent()
  local event = lost.guiro.event.Event(lost.guiro.event.Event.VALUE_CHANGED)
  event.bubbles = true
  event.target = self
  self:dispatchEvent(event)    
end
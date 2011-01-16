module("lost.guiro.view", package.seeall)

require("lost.guiro.view.View")

local Color = lost.common.Color

-- theme: the theme for the window
-- style: the style for the window
-- button: the button requesting the window
function lost.guiro.view.colorPickerWindow(wtheme, wstyle, button)
  -- windows are managed in a dict depending on their theme/style combination
  if not lost.guiro.view._colorPickerWindow then lost.guiro.view._colorPickerWindow = {} end
  if not lost.guiro.view._colorPickerWindow[wtheme] then lost.guiro.view._colorPickerWindow[wtheme] = {} end
  if not lost.guiro.view._colorPickerWindow[wtheme][wstyle] then
    lost.guiro.view._colorPickerWindow[wtheme][wstyle] = lost.guiro.view.ColorPickerWindow{theme=wtheme,style=wstyle}
  end
  local result = lost.guiro.view._colorPickerWindow[wtheme][wstyle]
  result:button(button)
  result:open()
end

lost.common.Class "lost.guiro.view.ColorPickerWindow" "lost.guiro.view.Window" {}

-- mandatory members set by style
-- * redSlider
-- * greenSlider
-- * blueSlider
-- * alphaslider
-- (all sliders expected to be min=0, max=1)
-- * redLabel
-- * greenLabel
-- * blueLabel
-- * alphaLabel
-- (labels will receive the numerical value of the corresponding color channel)
-- * colorLayer (will display the color currently defined by the sliders)
function ColorPickerWindow:constructor(args)
  lost.guiro.view.Window.constructor(self, args)
  local t = args or {}
  self.id = t.id or "colorpickerwindow"
  self:addEventListener("valueChanged", function(event) self:valueChanged(event) end)
end

function ColorPickerWindow:valueChanged(event)
  local c = Color(self.redSlider:value(),
                  self.greenSlider:value(),
                  self.blueSlider:value(),
                  self.alphaSlider:value())
  self._button:color(c)
end

function ColorPickerWindow:button(b)
  if self._button then
    self._button:pushed(false)
  end
  self._button = b
  self:title(b.name)
  self:color(b:color())
end

function ColorPickerWindow:close()
  if self._button then
    self._button:pushed(false)
  end
  lost.guiro.view.Window.close(self)
  self._button = nil
end

function ColorPickerWindow:color(val)
  self.redSlider:value(val.r)
  self.greenSlider:value(val.g)
  self.blueSlider:value(val.b)
  self.alphaSlider:value(val.a)
end



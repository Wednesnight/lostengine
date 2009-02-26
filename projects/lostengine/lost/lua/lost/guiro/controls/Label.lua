module("lost.guiro.controls", package.seeall)

require("lost.guiro.config")
require("lost.guiro.View")

--[[
     Label control
  ]]
Label = lost.common.Class("lost.guiro.controls.Label", lost.guiro.View)

--[[
    constructor
  ]]
function Label:__init() lost.guiro.View.__init(self)
  self.alpha = 1.0
  self.currentColor = lost.common.Color(1,1,1)
  self.font = lost.guiro.config.theme.defaultFont
  self.fontSize = 12
	self.textChanged = false
end

function Label:color(color)
  color = color or self.currentColor
  if self.currentColor ~= color then
    self.currentColor = lost.common.Color(color)
    self:needsRedraw()
  end
  return self.currentColor
end

function Label:setText(txt)
	self.text = txt
	self.textChanged = true
	self:needsRedraw()
end

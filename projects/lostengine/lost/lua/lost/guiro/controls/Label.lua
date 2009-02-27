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
function Label:__init(properties) lost.guiro.View.__init(self, properties)
  properties = properties or {}

  self.alpha = properties.alpha or 1.0
  self.currentColor = properties.color or lost.common.Color(1,1,1)
  self.font = properties.font or lost.guiro.config.theme.defaultFont
  self.fontSize = properties.fontSize or 12
	self.textChanged = false
	
	if properties.text then
	  self:setText(properties.text)
	end
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

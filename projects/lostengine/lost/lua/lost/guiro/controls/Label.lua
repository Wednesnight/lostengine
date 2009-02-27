module("lost.guiro.controls", package.seeall)

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

	self.textChanged = false
	if properties.text then
	  self:setText(properties.text)
	end
end

function Label:color(color)
  color = color or self.style.color
  if self.style.color ~= color then
    self.style.color = lost.common.Color(color)
    self:needsRedraw()
  end
  return self.style.color
end

function Label:setText(text)
	self.text = text
	self.textChanged = true
	self:needsRedraw()
end

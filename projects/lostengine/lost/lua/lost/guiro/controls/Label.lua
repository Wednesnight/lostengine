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

function Label:setText(text)
	self.text = text
	self.textChanged = true
	self:needsRedraw()
end

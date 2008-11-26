module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Label
  ]]

require("lost.io.Loader")
require("lost.guiro.controls.Label")

function Loader:Label(definition)
  local label = self:applyDefinitionToView(lost.guiro.controls.Label(), definition)
	if definition.text then
		label:setText(definition.text) -- renders the text 
	end
	return label
end

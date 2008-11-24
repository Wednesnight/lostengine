module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Label
  ]]

require("lost.io.Loader")
require("lost.guiro.controls.Label")

function Loader:Label(definition)
  if not definition.style then
    definition.style = self:StyleFromFile("lost/guiro/styles/default/Label.lua")
  end
  if not definition.renderer then
    definition.renderer = self:RendererFromFile("lost/guiro/renderer/default/Label.lua")
  end

  local label = self:applyDefinitionToView(lost.guiro.controls.Label(), definition)
	if definition.text then
		log.debug("SETTING TEXT");
		label:setText(definition.text) -- renders the text 
	end
	
	return label
end

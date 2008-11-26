module("lost.guiro.controls", package.seeall)

require("lost.guiro.config")
require("lost.guiro.controls.Control")

--[[
     Label control
  ]]
class "lost.guiro.controls.Label" (lost.guiro.controls.Control)
Label = _G["lost.guiro.controls.Label"]

lost.guiro.controls.Control:addBase(Label, "Label")

--[[
    constructor
  ]]
function Label:__init() super()
  self.alpha = 1.0
  self.color = lost.common.Color(1,1,1)
  self.font = lost.guiro.config.theme.defaultFont
  self.fontSize = 12
	self.textChanged = false
end

function Label:setText(txt)
	self.text = txt
	self.textChanged = true
end

module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: UserInterface
  ]]

require("lost.io.Loader")
require("lost.guiro.UserInterface")

function Loader:UserInterface(definition)
  return self:applyDefinitionToView(lost.guiro.UserInterface(), definition)
end

module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: UserInterface
  ]]

require("lost.io.Loader")

function Loader:UserInterface(definition)
  return self:applyDefinitionToView(lost.guiro.UserInterface(), definition)
end

module("lost.guiro", package.seeall)

--[[
     UserInterface class
  ]]
require("lost.guiro.View")

class "lost.guiro.UserInterface" (lost.guiro.View)
UserInterface = _G["lost.guiro.UserInterface"]

lost.guiro.View:addBase(UserInterface, "UserInterface")

function UserInterface:__init() super()
end

--[[
    only accepts lost.guiro.controls.Window
  ]]
function UserInterface:appendChild(child)
  if child:is("Window") then
    lost.guiro.View.appendChild(self, child)
  else
    log.error("UserInterface:appendChild() cannot append ".. child:className())
  end
end

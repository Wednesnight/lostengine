module("lost.guiro", package.seeall)

--[[
     UserInterface class
  ]]
require("lost.guiro.View")

class "lost.guiro.UserInterface" (lost.guiro.View)
UserInterface = _G["lost.guiro.UserInterface"]

lost.guiro.View:addBase(UserInterface, "UserInterface")

function UserInterface:__init() super()
  log.debug("UserInterface:__init()")
end

function UserInterface:dispatchEvent(event)
  log.debug("UserInterface:dispatchEvent()")
  lost.guiro.View.dispatchEvent(self, event)
end

function UserInterface:render()
--  log.debug("UserInterface:render()")
  lost.guiro.View.render(self)
end

function UserInterface:__finalize()
  log.debug("UserInterface:__finalize()")
end

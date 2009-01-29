module("lost.guiro", package.seeall)

--[[
     Screen class
  ]]
require("lost.guiro.View")
require("lost.guiro.EventManager")

class "lost.guiro.Screen" (lost.guiro.View)
Screen = _G["lost.guiro.Screen"]

lost.guiro.View:addBase(Screen, "Screen")

function Screen:propagateMouseEvent(event)
  self.eventManager:propagateMouseEvent(self, event)
end

function Screen:__init() lost.guiro.View.__init(self)
  log.debug("----------------------------------------------------------")
  self.eventManager = lost.guiro.EventManager()
  log.debug("----------------------------------------------------------")
  
  self:addEventListener(lost.application.MouseEvent.MOUSE_UP, function(event) self:propagateMouseEvent(event) end)
  self:addEventListener(lost.application.MouseEvent.MOUSE_DOWN, function(event) self:propagateMouseEvent(event) end)
--  self:addEventListener(lost.application.MouseEvent.MOUSE_MOVE, function(event) self:propagateMouseEvent(event) end)
end

--[[
    only accepts lost.guiro.UserInterface
  ]]
function Screen:appendChild(child)
  if child:is("UserInterface") then
    lost.guiro.View.appendChild(self, child)
  else
    log.error("Screen:appendChild() cannot append ".. child:className())
  end
end

--[[
    Screen is always dirty
  ]]
function Screen:render(context, forceRender)
  for k,ui in next,self.children do
    ui:render(context, forceRender)
  end
end

module("lost.guiro", package.seeall)

require("lost.guiro.View")

--[[
     UserInterface class
  ]]
lost.common.Class "lost.guiro.UserInterface" "lost.guiro.View" {}

function UserInterface:constructor()
  lost.guiro.View.constructor(self)
  
  self:addEventListener(lost.application.MouseEvent.MOUSE_DOWN, function(event) self:updateFocus(event) end)
end

--[[
    only accepts lost.guiro.controls.Window
  ]]
function UserInterface:addSubview(subview)
  if subview:isDerivedFrom("lost.guiro.Window") then
    lost.guiro.View.addSubview(self, subview)
  else
    error("UserInterface:addSubview() can only add subviews of type Window : ".. subview:className(), 2)    
  end
end

--[[
    get focused window
  ]]
function UserInterface:topWindow()
  return self.subviews[table.maxn(self.subviews)]
end

--[[
    reorders windows by bringing focused window to front
  ]]
function UserInterface:updateFocus(event)
  local mouseEvent = event
  local topWindow = self:topWindow()
  if not topWindow or not topWindow:containsCoord(mouseEvent.pos) then
    local idx = table.maxn(self.subviews)
    while idx > 0 do
      local subview = self.subviews[idx]
      if subview:containsCoord(mouseEvent.pos) then
        self:removeSubview(subview)
        self:addSubview(subview)
        subview:needsRedraw()
        break
      end
      idx = idx-1
    end
  end
end

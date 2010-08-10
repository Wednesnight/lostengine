module("lost.guiro.view", package.seeall)

require("lost.guiro.view.View")

--[[
     UserInterface class
  ]]
lost.common.Class "lost.guiro.view.UserInterface" "lost.guiro.view.View" {}

function UserInterface:constructor(textureManager)
  lost.guiro.view.View.constructor(self, textureManager)
  self:addEventListener(lost.application.MouseEvent.MOUSE_DOWN, function(event) self:updateFocus(event) end)
end

--[[
    only accepts lost.guiro.controls.Window
  ]]
function UserInterface:addSubview(subview)
  if subview:isDerivedFrom("lost.guiro.view.Window") then
    lost.guiro.view.View.addSubview(self, subview)
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
      if subview.focusable and subview:containsCoord(mouseEvent.pos) then
        table.remove(self.subviews, idx)
        table.insert(self.subviews, subview)
        self:needsRedraw()
        break
      end
      idx = idx-1
    end
  end
end

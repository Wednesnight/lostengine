module("lost.guiro.view", package.seeall)

require("lost.guiro.view.View")

--[[
     UserInterface class
  ]]
lost.common.Class "lost.guiro.view.UserInterface" "lost.guiro.view.View" {}

function UserInterface:constructor(textureManager)
  lost.guiro.view.View.constructor(self, textureManager)
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

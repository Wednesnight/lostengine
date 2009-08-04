-- lost.declarative.Guiro
module("lost.declarative", package.seeall)

require("lost.guiro.Screen")
require("lost.guiro.UserInterface")
require("lost.guiro.Window")
require("lost.guiro.View")

lost.common.Class "lost.declarative.Guiro" {}

function Guiro:constructor(loader)
  self.loader = loader
end

-- searches i ntable "source" for classes derived from View and adds
-- them to target as subviews 
function Guiro:searchAndAddSubviews(target, source)
  for k,v in pairs(source) do
    if (type(v) == "table") and (v.isDerivedFrom ~= nil) and (v:isDerivedFrom("lost.guiro.View")) then
      target:addSubview(v)
    end
  end
end

function Guiro:Screen(def)
  local result = lost.guiro.Screen()
  self:searchAndAddSubviews(result, def)  
  return result
end

function Guiro:View(def)
  local result = lost.guiro.View()
  self:searchAndAddSubviews(result, def)    
  return result
end

function Guiro:UserInterface(def)
  local result = lost.guiro.UserInterface()
  self:searchAndAddSubviews(result, def)      
  return result
end

function Guiro:Window(def)
  local result = lost.guiro.Window()
  self:searchAndAddSubviews(result, def)  
  return result
end
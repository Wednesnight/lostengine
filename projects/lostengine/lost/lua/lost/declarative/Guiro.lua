-- lost.declarative.Guiro
module("lost.declarative", package.seeall)

require("lost.guiro.Screen")
require("lost.guiro.UserInterface")
require("lost.guiro.Window")
require("lost.guiro.View")
require("lost.guiro.Label")

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

function Guiro:assignViewAttributes(target, source)
  if source.bounds ~= nil then
    target.bounds = source.bounds
  end
  if source.id ~= nil then
    target.id = source.id
  end
  if source.showFrame ~= nil then
    target:showFrame(source.showFrame)
  end
  if source.frameColor ~= nil then
    target:frameColor(source.frameColor)
  end
  if source.showBackground ~= nil then
    target:showBackground(source.showBackground)
  end
  if source.backgroundColor ~= nil then
    target:backgroundColor(source.backgroundColor)
  end
  target:needsLayout()
end

function Guiro:assignLabelAttributes(target, source)
  if source.text then target:text(source.text) end
  if source.textColor then target:textColor(source.textColor) end
  if source.font then target:font(source.font) end
  if source.fontSize then target:fontSize(source.fontSize) end
  if source.showShadow then target:showShadow(source.showShadow) end
  if source.shadowOffset then target:shadowOffset(source.shadowOffset) end
  if source.shadowColor then target:shadowColor(source.shadowColor) end
  if source.halign then target:halign(source.halign) end
  if source.valign then target:valign(source.valign) end
end

-- if source contains a key 'listeners' with a table as value
-- the keys in that table are event names and the values are handlers.
-- the handlers are then registered as target/bubble listeners.
function Guiro:addEventListeners(target, source)
  if source.listeners == nil then return end
  for k,v in pairs(source.listeners) do
    target:addEventListener(k,v)
  end
end

function Guiro:Screen(def)
  local result = lost.guiro.Screen()
  self:searchAndAddSubviews(result, def) 
  self:assignViewAttributes(result, def) 
  self:addEventListeners(result, def)
  return result
end

function Guiro:View(def)
  local result = lost.guiro.View()
  self:searchAndAddSubviews(result, def)    
  self:assignViewAttributes(result, def) 
  self:addEventListeners(result, def)
  return result
end

function Guiro:UserInterface(def)
  local result = lost.guiro.UserInterface()
  self:searchAndAddSubviews(result, def)      
  self:assignViewAttributes(result, def) 
  self:addEventListeners(result, def)
  return result
end

function Guiro:Window(def)
  local result = lost.guiro.Window()
  self:searchAndAddSubviews(result, def)  
  self:assignViewAttributes(result, def) 
  self:addEventListeners(result, def)
  return result
end

function Guiro:Label(def)
  local result = lost.guiro.Label()
  -- don't allow label subviews
  self:assignViewAttributes(result, def) 
  self:assignLabelAttributes(result, def)   
  self:addEventListeners(result, def)
  return result
end
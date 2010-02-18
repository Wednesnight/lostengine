-- lost.declarative.Guiro
module("lost.declarative", package.seeall)

require("lost.guiro.Screen")
require("lost.guiro.UserInterface")
require("lost.guiro.Window")
require("lost.guiro.View")
require("lost.guiro.Label")
require("lost.guiro.Image")
require("lost.guiro.Button")
require("lost.guiro.VBox")
require("lost.guiro.HBox")
require("lost.guiro.RenderView")
require("lost.guiro.ThemeManager")
require("lost.guiro.event.Event")

lost.common.Class "lost.declarative.Guiro" {}

using "lost.guiro.event.Event"

function Guiro:constructor(loader, themeManager)
  self.loader = loader
  self.themeManager = themeManager or lost.guiro.ThemeManager(loader)
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
  if source.bounds ~= nil then target.bounds = source.bounds end
  if source.id  ~= nil then target.id = source.id end
  if source.showFrame  ~= nil then target:showFrame(source.showFrame)	end
  if source.frameColor  ~= nil then target:frameColor(source.frameColor) end
  if source.showBackground  ~= nil then	target:showBackground(source.showBackground)	end
  if source.backgroundColor  ~= nil then target:backgroundColor(source.backgroundColor) end
  target:needsRedraw()
  target:needsLayout()
end

function Guiro:assignLabelAttributes(target, source)
  if source.text  ~= nil then target:text(source.text) end
  if source.textColor  ~= nil then target:textColor(source.textColor) end
  if source.font  ~= nil then target:font(source.font) end
  if source.fontSize  ~= nil then target:fontSize(source.fontSize) end
  if source.showShadow  ~= nil then target:showShadow(source.showShadow) end
  if source.shadowOffset  ~= nil then target:shadowOffset(source.shadowOffset) end
  if source.shadowColor  ~= nil then target:shadowColor(source.shadowColor) end
  if source.halign  ~= nil then target:halign(source.halign) end
  if source.valign  ~= nil then target:valign(source.valign) end
end

function Guiro:assignImageAttributes(target, source)
  if (source.bitmap and source.texture) or (source.bitmap and source.filename) or
     (source.texture and source.filename)
  then
    error("you can only specify bitmap, texture OR filename for the creation of a guiro Image", 2)
  end

  if source.bitmap  ~= nil then target:bitmap(source.bitmap) end
  if source.filename  ~= nil then target:bitmap(self.loader:load(source.filename)) end
  if source.texture  ~= nil then target:texture(source.texture) end
  if source.scale  ~= nil then target:scale(source.scale) end
  if source.caps  ~= nil then target:caps(source.caps) end
  if source.filter  ~= nil then target:filter(source.filter) end
  if source.flip  ~= nil then target:flip(source.flip) end
end

function Guiro:assignButtonAttributes(target, source)
	if source.title ~= nil then target:title(source.title) end
end

function Guiro:assignVBoxAttributes(target, source)
	if source.mode ~= nil then target:mode(source.mode) end
	if source.halign ~= nil then target:halign(source.halign) end
	if source.valign ~= nil then target:valign(source.valign) end
	if source.spacing ~= nil then target:spacing(source.spacing) end
end

function Guiro:assignHBoxAttributes(target, source)
	if source.mode ~= nil then target:mode(source.mode) end
	if source.halign ~= nil then target:halign(source.halign) end
	if source.valign ~= nil then target:valign(source.valign) end
	if source.spacing ~= nil then target:spacing(source.spacing) end
end

function Guiro:assignRenderViewAttributes(target, source)
  if type(source.rendergraph) == "table" then
    for k,node in next,source.rendergraph do
      target:renderGraph():add(node)
    end
  end
end

function Guiro:applyStyle(target, def)
  local themeName = def.theme or self.themeManager.defaultTheme
  local styleName = def.style or self.themeManager.defaultStyle
  self.themeManager:apply(target, themeName, styleName)
end

-- if source contains a key 'listeners' with a table as value
-- the keys in that table are event names and the values are handlers.
-- the handlers are then registered as target/bubble listeners.
function Guiro:addEventListeners(target, source)
  if source.listeners ~= nil then
    -- capture listeners
    if source.listeners.capture ~= nil then
      for k,v in pairs(source.listeners.capture) do
        target:addEventListener(k,v,Event.PHASE_CAPTURE)
      end
    end
    -- target listeners (default)
    if source.listeners.target ~= nil then
      for k,v in pairs(source.listeners.target) do
        target:addEventListener(k,v)
      end
    end
    -- bubble listeners
    if source.listeners.bubble ~= nil then
      for k,v in pairs(source.listeners.bubble) do
        target:addEventListener(k,v,Event.PHASE_BUBBLE)
      end
    end
    -- default (target)
    for k,v in pairs(source.listeners) do
      if type(v) == "function" then
        target:addEventListener(k,v)
      end
    end
  end
end

function Guiro:View(def)
  local result = lost.guiro.View()
  self:applyStyle(result, def)
  self:searchAndAddSubviews(result, def)    
  self:assignViewAttributes(result, def) 
  self:addEventListeners(result, def)
  return result
end

function Guiro:Screen(def)
  local result = lost.guiro.Screen()
  self:applyStyle(result, def)
  self:searchAndAddSubviews(result, def) 
  self:assignViewAttributes(result, def) 
  self:addEventListeners(result, def)
  return result
end

function Guiro:UserInterface(def)
  local result = lost.guiro.UserInterface()
  self:applyStyle(result, def)
  self:searchAndAddSubviews(result, def)      
  self:assignViewAttributes(result, def) 
  self:addEventListeners(result, def)
  return result
end

function Guiro:Window(def)
  local result = lost.guiro.Window()
  self:applyStyle(result, def)
  self:searchAndAddSubviews(result, def)  
  self:assignViewAttributes(result, def) 
  self:addEventListeners(result, def)
  return result
end

function Guiro:Label(def)
  local result = lost.guiro.Label()
  self:applyStyle(result, def)
  -- don't allow label subviews
  self:assignViewAttributes(result, def) 
  self:assignLabelAttributes(result, def)   
  self:addEventListeners(result, def)
  return result
end

function Guiro:Image(def)
  local result = lost.guiro.Image()
  self:applyStyle(result, def)
  -- don't allow image subviews
  self:assignViewAttributes(result, def) 
  self:assignImageAttributes(result, def)   
  self:addEventListeners(result, def)
  return result
end

function Guiro:Button(def)
  local result = lost.guiro.Button()
  self:applyStyle(result, def)
  -- don't allow image subviews
  self:assignViewAttributes(result, def) 
  self:assignButtonAttributes(result, def)   
  self:addEventListeners(result, def)
  return result
end

function Guiro:VBox(def)
  local result = lost.guiro.VBox()
  self:applyStyle(result, def)
  self:searchAndAddSubviews(result, def)  
  self:assignViewAttributes(result, def) 
  self:assignVBoxAttributes(result, def)   
  self:addEventListeners(result, def)
  return result
end

function Guiro:HBox(def)
  local result = lost.guiro.HBox()
  self:applyStyle(result, def)
  self:searchAndAddSubviews(result, def)  
  self:assignViewAttributes(result, def) 
  self:assignHBoxAttributes(result, def)   
  self:addEventListeners(result, def)
  return result
end

function Guiro:RenderView(def)
  local result = lost.guiro.RenderView()
  self:applyStyle(result, def)
  -- don't allow subviews
  self:assignViewAttributes(result, def)
  self:assignImageAttributes(result, def)
  self:assignRenderViewAttributes(result, def)
  self:addEventListeners(result, def)
  return result
end

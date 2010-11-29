-- lost.declarative.Guiro
module("lost.declarative", package.seeall)

require("lost.guiro.view.UserInterface")
require("lost.guiro.view.View")
require("lost.guiro.view.Label")
require("lost.guiro.view.Image")
require("lost.guiro.view.Button")
require("lost.guiro.view.VBox")
require("lost.guiro.view.HBox")
require("lost.guiro.view.RenderView")
require("lost.guiro.view.SpinEdit")
require("lost.guiro.ThemeManager")
require("lost.guiro.event.Event")

lost.common.Class "lost.declarative.Guiro" {}

using "lost.guiro.event.Event"

function Guiro:constructor(loader, themeManager, textureManager)
  self.loader = loader
  self.themeManager = themeManager 
  self.textureManager = textureManager
end

-- searches in table "source" for classes derived from View and adds
-- them to target as subviews 
function Guiro:searchAndAddSubviews(target, source)
  for k,v in pairs(source) do
    if (type(v) == "table") and (v.isDerivedFrom ~= nil) and (v:isDerivedFrom("lost.guiro.view.View")) then
      target:addSubview(v)
    end
  end
end

-- in order to enable the setting of parts of the bounds from theme and declarative guiro constructs,
-- we need to carefully merge the bounds. Creation happens as follows:
-- * the View class constructs default bounds for itself, which are initially fullscreen, so you can see at least something
-- * derived classes can override this in their constructor
-- * a style can modify the bounds of the target view 
-- * the last step is the application of the declarative bounds instance
-- during that last step, the bounds should be carefully merged, so that the style can create a temporary bounds
-- object which is partially filled, then the rest of the bounds parameters can be copied from the declarative instance.
-- in order to help debug broken constructs , we should report bounds objects that are incomplete after the merge
function mergeBounds(targetBounds, sourceBounds)
  if sourceBounds == nil then return end
  targetBounds:merge(sourceBounds)
  if not targetBounds:complete() then
    log.error("incomplete bounds detected after merge")
  end
end

function Guiro:assignViewAttributes(target, source)

  if source.bounds and source.bounds.isDerivedFrom and source.bounds:isDerivedFrom("lost.guiro.Bounds") then
    mergeBounds(target.bounds, source.bounds)
  elseif source.bounds and (type(source.bounds)=="table") then
--    local t = source.bounds
--    local b = lost.guiro.Bounds(t[1],t[2],t[3],t[4]) -- decodes the values from the input table
--    mergeBounds(target.bounds, b) -- we still need to merge    
    mergeBounds(target.bounds, lost.guiro.Bounds(unpack(source.bounds))) -- we still need to merge    
  end
  
  if source.id  ~= nil then target.id = source.id end
  if source.showFrame  ~= nil then target:showFrame(source.showFrame)	end
  if source.frameColor  ~= nil then target:frameColor(source.frameColor) end
  if source.showBackground  ~= nil then	target:showBackground(source.showBackground)	end
  if source.backgroundColor  ~= nil then target:backgroundColor(source.backgroundColor) end
  if source.hidden  ~= nil then target:hidden(source.hidden) end
  if source.frameWidth  ~= nil then target:frameWidth(source.frameWidth) end
  if source.backgroundCornerRadius  ~= nil then target:backgroundCornerRadius(source.backgroundCornerRadius) end
  if source.frameCornerRadius  ~= nil then target:frameCornerRadius(source.frameCornerRadius) end
  if source.backgroundRoundCorners ~= nil then 
    local t = source.backgroundRoundCorners
    target:backgroundRoundCorners(t[1],t[2],t[3],t[4]) 
  end
  if source.frameRoundCorners ~= nil then 
    local t = source.frameRoundCorners
    target:frameRoundCorners(t[1],t[2],t[3],t[4]) 
  end
  if source.frameShowSides ~= nil then 
    local t = source.frameShowSides
    target:frameShowSides(t[1],t[2],t[3],t[4]) 
  end
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
	if source.enabled ~= nil then target:enabled(source.enabled) end
	if source.mode ~= nil then target:mode(source.mode) end
	if source.pushed ~= nil then target:pushed(source.pushed) end
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

function Guiro:assignSpinEditAttributes(target, source)
  if source.value ~= nil then target:value(source.value) end
  if source.min ~= nil then target:min(source.min) end
  if source.max ~= nil then target:max(source.max) end
  if source.stepSize ~= nil then target:stepSize(source.stepSize) end
end

function Guiro:applyStyle(target, def)
  local themeName = def.theme or self.themeManager.defaultTheme
  local styleName = def.style or self.themeManager.defaultStyle
  self.themeManager:apply(target, themeName, styleName, def)
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
  local result = lost.guiro.view.View(self.textureManager)
  self:applyStyle(result, def)
  self:searchAndAddSubviews(result, def)    
  self:assignViewAttributes(result, def) 
  self:addEventListeners(result, def)
  return result
end

function Guiro:UserInterface(def)
  local result = lost.guiro.view.UserInterface(self.textureManager)
  if _G.ui == nil then
    _G.ui = result
  end
  self:applyStyle(result, def)
  self:searchAndAddSubviews(result, def)      
  self:assignViewAttributes(result, def) 
  self:addEventListeners(result, def)
  return result
end

function Guiro:Label(def)
  local result = lost.guiro.view.Label(self.textureManager)
  self:applyStyle(result, def)
  -- don't allow label subviews
  self:assignViewAttributes(result, def) 
  self:assignLabelAttributes(result, def)   
  self:addEventListeners(result, def)
  return result
end

function Guiro:Image(def)
  local result = lost.guiro.view.Image(self.textureManager)
  self:applyStyle(result, def)
  -- don't allow image subviews
  self:assignViewAttributes(result, def) 
  self:assignImageAttributes(result, def)   
  self:addEventListeners(result, def)
  return result
end

function Guiro:Button(def)
  local result = lost.guiro.view.Button(self.textureManager)
  self:applyStyle(result, def)
  -- don't allow image subviews
  self:assignViewAttributes(result, def) 
  self:assignButtonAttributes(result, def)   
  self:addEventListeners(result, def)
  return result
end

function Guiro:VBox(def)
  local result = lost.guiro.view.VBox(self.textureManager)
  self:applyStyle(result, def)
  self:searchAndAddSubviews(result, def)  
  self:assignViewAttributes(result, def) 
  self:assignVBoxAttributes(result, def)   
  self:addEventListeners(result, def)
  return result
end

function Guiro:HBox(def)
  local result = lost.guiro.view.HBox(self.textureManager)
  self:applyStyle(result, def)
  self:searchAndAddSubviews(result, def)  
  self:assignViewAttributes(result, def) 
  self:assignHBoxAttributes(result, def)   
  self:addEventListeners(result, def)
  return result
end

function Guiro:RenderView(def)
  local result = lost.guiro.view.RenderView(self.textureManager)
  self:applyStyle(result, def)
  -- don't allow subviews
  self:assignViewAttributes(result, def)
  self:assignImageAttributes(result, def)
  self:assignRenderViewAttributes(result, def)
  self:addEventListeners(result, def)
  return result
end

function Guiro:SpinEdit(def)
  local result = lost.guiro.view.SpinEdit(self.textureManager)
  self:applyStyle(result, def)
  -- don't allow subviews
  self:assignViewAttributes(result, def)
  self:assignSpinEditAttributes(result, def)
  self:addEventListeners(result, def)
  return result
end

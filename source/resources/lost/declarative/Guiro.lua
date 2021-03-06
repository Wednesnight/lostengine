-- lost.declarative.Guiro
module("lost.declarative", package.seeall)

require("lost.guiro.view.UserInterface")
require("lost.guiro.view.View")
require("lost.guiro.view.Label")
require("lost.guiro.view.Button")
require("lost.guiro.ThemeManager")
require("lost.guiro.event.Event")

lost.common.Class "lost.declarative.Guiro" {}

local Event = lost.guiro.event.Event

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
  if source.hidden  ~= nil then target:hidden(source.hidden) end
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

function Guiro:assignButtonAttributes(target, source)
	if source.title ~= nil then target:title(source.title) end
	if source.enabled ~= nil then target:enabled(source.enabled) end
	if source.mode ~= nil then target:mode(source.mode) end
	if source.pushed ~= nil then target:pushed(source.pushed) end
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
  local result = lost.guiro.view.UserInterface()
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

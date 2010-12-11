module("lost.guiro.view", package.seeall)

require("lost.guiro.view.View")
require("lost.guiro.event.Event")
require("lost.guiro.layout.Horizontal")

lost.common.Class "lost.guiro.view.TabBar" "lost.guiro.view.View" {}

function TabBar:constructor(args)
  local t = args or {}

  -- style params that will be passed on to Buttons for styling, shuld be set in theme
  self.singleButtonStyleParams = {}
  self.leftButtonStyleParams = {}
  self.midButtonStyleParams = {}
  self.rightButtonStyleParams = {}

  -- button size = rendered text width + buttonSizeAdjust
  -- buttons will be created with default size first, text rendered, then button size readjusted
  self.buttonSizeAdjust = 20 -- 

  self._buttons = {} -- buttons as created from items
  
	lost.guiro.view.View.constructor(self, args) 
  self.id = t.id or "tabBar"
  self._items = t.items or {} -- items as passed in constructor, will be used for button creation, currently only a list of strings
  self:rebuildButtons()
  self.layout = lost.guiro.layout.Horizontal{valign="center", halign="center"} -- buttons are always stacked horizontally, tightly packed and centered in all directions
  self.selected = -1
  self:select(t.selected or 1)
  self:needsLayout()
end

function TabBar:adjustButtonWidth(button, item)
  local width = 0
  if button.textLayer then
    local rt = button.textLayer._font:render(item)
    width = rt.size.width + self.buttonSizeAdjust
    log.debug("-- NEWWIDTH: "..width)
  end
  button._bounds.width = lost.guiro.Bounds.decodeEntry(3,width) 
end

local function mergeTables(dest,source)
  for k,v in pairs(source) do
    dest[k] = v
  end
end

function TabBar:addStyleParams(params, index)
  if (index == 1) and (#self._items == 1) then
    mergeTables(params, self.singleButtonStyleParams)
  elseif index == 1 then
    mergeTables(params, self.leftButtonStyleParams)
  elseif index == #self._items then
    mergeTables(params, self.rightButtonStyleParams)
  else
    mergeTables(params, self.midButtonStyleParams)
  end
end

-- rebuild buttons from items
function TabBar:rebuildButtons()
  -- remove all previous buttons
  for _,v in pairs(self._buttons) do
    v:removeFromSuperview()
  end
  
  -- create new ones from items
  for index,item in ipairs(self._items) do
    local params = {bounds={0,0,50,10},title=item}
    self:addStyleParams(params, index)
    local button = lost.guiro.view.Button(params)
    self:adjustButtonWidth(button, item)
    self:addSubview(button)
    table.insert(self._buttons, button)
  end
  self:needsLayout()
end

function TabBar:update()
  for index,button in ipairs(self._buttons) do
    if index == self.selected then
      button:pushed(true)
    else
      button:pushed(false)
    end
  end
end

-- sets the current tabbar selection, 1-based
function TabBar:select(v)
  if v ~= self.selected then
    self.selected = v
    self:needsUpdate()
  end
end

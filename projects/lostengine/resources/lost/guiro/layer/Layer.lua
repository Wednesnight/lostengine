module("lost.guiro.layer", package.seeall)

require("lost.guiro.Bounds")

lost.common.Class "lost.guiro.layer.Layer" {}

local Rect = lost.math.Rect
local Bounds = lost.guiro.Bounds

-- optionally set id and bounds in args
function Layer:constructor(args)
  local t = args or {}
	self._renderNode = lost.rg.Node.create()
	self._superlayer = nil
  self.sublayers = {}
	self.z = 0
	self.id = t.id or "layer"
	if t.bounds then 
	  self:bounds(Bounds(unpack(t.bounds)))
	else
	  self:bounds(Bounds("left", "bottom", "1", "1"))
	end
	self.rect = Rect()
end

function Layer:bounds(...)
  if arg.n > 0 then
    self._bounds = arg[1]
    self:needsLayout()
  else
    return self._bounds
  end
  self:updateZ()
  self:needsLayout()
end

function Layer:superlayer(...)
  if arg.n > 0 then
    local newsl = arg[1]
--    log.debug("-- "..self.id.." setting superlayer to "..tostring(newsl))
    self._superlayer = newsl
  else
    return self._superlayer
  end
  self:updateZ()  
end

function Layer:updateZ()
  if self._superlayer then 
    self.z = self._superlayer.z+1
  else
    self.z = 0
  end
  self:updateSublayerZ()
end

function Layer:updateSublayerZ()
  for _,v in pairs(self.sublayers) do
    v.z = self.z+1
    v:updateSublayerZ()
  end
end

function Layer:addSublayer(layer)
  if layer:superlayer() ~= nil then layer:removeFromSuperlayer() end
  table.insert(self.sublayers, layer)
  self._renderNode:add(layer._renderNode)
  layer:superlayer(self)
  layer:needsLayout()
end

function Layer:removeSublayer(layer)
  for k,v in pairs(self.sublayers) do
    if rawequal(v,layer) then
      self.sublayers[k] = nil
      self._renderNode:remove(layer._renderNode)
      layer:superlayer(nil)
      break
    end
  end
end

function Layer:removeFromSuperlayer()
  if self._superlayer then
    self._superlayer:removeSublayer(self)
  end
end

function Layer:removeAllSublayers()
  while (#self.sublayers) > 0 do
    local l = self.sublayers[1]
    l:removeFromSuperlayer()
  end
end

function Layer:print(prefix)
  if not prefix then
    prefix = ""
  end
  log.debug(prefix .."|--("..self.z..") ".. self.id)
  for k,view in pairs(self.sublayers) do
    view:print(prefix .."    ")
  end
end

function Layer:__tostring()
  return self:className() .."(".. self.id ..")"
end

function Layer:needsUpdate()
  ui:layerNeedsUpdate(self)
end

function Layer:needsLayout()
  ui:layerNeedsLayout(self)
end

function Layer:needsDisplay()
  ui:layerNeedsDisplay(self)
end

function Layer:update()
  log.debug("-- layer update ("..self.z..") "..self.id)
end

function Layer:updateLayout()
  log.debug("-- layer update layout ("..self.z..") "..self.id)
--[[  local slid = nil
  if self._superlayer then
    slid = self._superlayer.id
  end
  log.debug("-- layer update layout ("..self.z..") "..self.id.." superlayer "..tostring(slid))
  ]]
  
  local superrect = nil
  if self._superlayer then
    superrect = self._superlayer.rect
  else
--    log.debug("!! no superlayer")
    superrect = Rect()
  end  
--  log.debug("updating with superrect: "..tostring(superrect))
  self.rect = self._bounds:rect(superrect)
--  log.debug(tostring(self.rect))
end

function Layer:updateDisplay()
  log.debug("-- layer update display ("..self.z..") "..self.id)
end
module("lost.guiro.layer", package.seeall)

require("lost.guiro.Bounds")

lost.common.Class "lost.guiro.layer.Layer" {}

local Rect = lost.math.Rect
local Bounds = lost.guiro.Bounds

-- valid params:
-- * id: the name of the layer, also used for naming the renderNode for debug display
-- * bounds: the bounds of the layer, default is (0,0,"1", "1")
-- * sublayers: additional layers that will be added as sublayers to the current one
function Layer:constructor(args)
  local t = args or {}
	self.id = t.id or "layer"
	self.renderNode = lost.rg.Node.create()
	self.renderNode.name = self.id
	self.layerNodes = lost.rg.Node.create() -- add draw for this layer to this node so sublayers are drawn after this layer
	self.layerNodes.name = "layerNodes"
	self.renderNode:add(self.layerNodes)
	self._superlayer = nil
  self.sublayers = {}
	self.z = 0
	
	if t.bounds then 
	  self:bounds(Bounds(unpack(t.bounds)))
	else
	  self:bounds(Bounds(0, 0, "1", "1"))
	end
	self.rect = Rect()
	
	local layers = t.sublayers or {}
	for _,v in pairs(layers) do
	  self:addSublayer(v)
	end
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
  self.renderNode:add(layer.renderNode)
  layer:superlayer(self)
  layer:needsLayout()
end

function Layer:removeSublayer(layer)
  for k,v in pairs(self.sublayers) do
    if rawequal(v,layer) then
      self.sublayers[k] = nil
      self.renderNode:remove(layer.renderNode)
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
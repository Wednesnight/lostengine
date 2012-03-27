module("lost.guiro.layer", package.seeall)

require("lost.guiro.Bounds")
require("lost.guiro.UpdateManager")

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
	self.renderNode = lost.rg.ScissorStack.create()
	self.renderNode.name = self.id
	self.layerNodes = lost.rg.Node.create() -- add draw for this layer to this node so sublayers are drawn after this layer
	self.layerNodes.name = "layerNodes"
	self.renderNode:add(self.layerNodes)
	self._superlayer = nil
  self.sublayers = {}
	self.z = 0
	
	if t.hidden then self:hidden(t.hidden) end
	if t.bounds then 
	  self:bounds(Bounds(unpack(t.bounds)))
	else
	  self:bounds(Bounds(0, 0, "1", "1"))
	end
	self.rect = Rect()
	if t.layout then
	  self.layout = t.layout 
	else
    self.layout = nil
	end
	
	if t.clip then
	  self:clip(t.clip)
	end
	
	local layers = t.sublayers or {}
	for _,v in pairs(layers) do
	  self:addSublayer(v)
	end
	
	self._bbcounter = lost.profiler.BBCount("lost.guiro.layer.Layer")
end

function Layer:bounds(...)
  if arg.n > 0 then
    self._bounds = arg[1]
    self:needsLayout()
  else
    return self._bounds
  end
end

function Layer:x(v)
  self._bounds.x = lost.guiro.Bounds.decodeEntry(1,v)
  self:needsLayout()
end

function Layer:y(v)
  self._bounds.y = lost.guiro.Bounds.decodeEntry(2,v)
  self:needsLayout()
end

function Layer:width(v)
  self._bounds.width = lost.guiro.Bounds.decodeEntry(3,v)
  self:needsLayout()
end

function Layer:height(v)
  self._bounds.height = lost.guiro.Bounds.decodeEntry(4,v)
  self:needsLayout()
end

function Layer:pos(xv,yv)
  self:x(xv)
  self:y(yv)
end

function Layer:size(wv,hv)
  self:width(wv)
  self:height(hv)
end

function Layer:superRect()
  local result = Rect()
  if self._superlayer then
    result = self._superlayer.rect
  end
  return result
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
    local l = self.sublayers[#self.sublayers]
    l:removeFromSuperlayer()
  end
end

function Layer:print(prefix)
  if not prefix then
    prefix = ""
  end
  log.debug(prefix .."|--("..self.z..") ".. self.id.." "..tostring(self.rect))
  for k,view in pairs(self.sublayers) do
    view:print(prefix .."    ")
  end
end

function Layer:__tostring()
  return self:className() .."(".. self.id ..")"
end

function Layer:needsUpdate()
  lost.guiro.updateManager():layerNeedsUpdate(self)
end

function Layer:needsLayout()
  lost.guiro.updateManager():layerNeedsLayout(self)
  for _,l in pairs(self.sublayers) do
    l:needsLayout()
  end
end

function Layer:needsDisplay()
  lost.guiro.updateManager():layerNeedsDisplay(self)
end

function Layer:update()
--  log.debug("-- layer update ("..self.z..") "..self.id)
end

function Layer:updateRect()
--  log.debug("updateRect "..self.id)
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

function Layer:updateLayout()
--  log.debug("updateLayout "..self.id)
  self:updateRect()
  self.renderNode.rect = self.rect
end

function Layer:updateDisplay()
--  log.debug("-- layer update display ("..self.z..") "..self.id)
end

function Layer:__call(layerId)
  local result = nil
  for k,layer in pairs(self.sublayers) do
    if (layer.id == layerId) then
      result = layer
      break
    end
  end
  return result
end

--breadth-first search for subview with given id
function Layer:recursiveFindById(layerId)
  local result = self(layerId)
  if result == nil then
    for k,layer in pairs(self.sublayers) do
      result = layer:recursiveFindById(layerId)
      if result ~= nil then
        break
      end
    end
  end
  return result
end

-- true to hide layer, false to show, no params to get current setting
function Layer:hidden(...)
  if arg.n>=1 then
    self.renderNode.active = not arg[1]
  else
    return (not self.renderNode.active) or ((self._superlayer ~= nil) and self._superlayer:hidden())
  end
end

function Layer:show()
  self:hidden(false)
end

function Layer:hide()
  self:hidden(true)
end

function Layer:clip(...)
  if arg.n >= 1 then
    self.renderNode.clip = arg[1]
  else
    return self.renderNode.clip
  end
end

function Layer:color(...)
  if arg.n >= 1 then
    self._color = arg[1]
  else
    return self._color
  end
end

function Layer:containsPoint(point)
  return self.rect:contains(point)
end


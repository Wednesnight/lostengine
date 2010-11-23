module("lost.guiro.layer", package.seeall)

lost.common.Class "lost.guiro.layer.Layer" {}

function Layer:constructor()
	self._renderNode = lost.rg.Node.create()
	self._superLayer = nil
  self._subLayers = {}
	self._z = 0
	self.id = "layer"
end

function Layer:superLayer(...)
  if arg.n > 0 then
    local newsl = arg[1]
    self._superLayer = newsl
  else
    return self._superLayer
  end
end

function Layer:addSubLayer(layer)
  if layer:superLayer() ~= nil then layer:removeFromSuperLayer() end
  table.insert(self._subLayers, layer)
  self._renderNode:add(layer._renderNode)
end

function Layer:removeLayer(layer)
  for k,v in pairs(self._subLayers) do
    if rawequal(v,layer) then
      self._subLayers[k] = nil
      self._renderNode:remove(layer._renderNode)
      layer:superLayer(nil)
      break
    end
  end
end

function Layer:removeFromSuperLayer()
  if self._superLayer then
    self._superLayer:removeLayer(self)
  end
end

function Layer:removeAllSubLayers()
  while (#self._subLayers) > 0 do
    local l = self._subLayers[1]
    l:removeFromSuperLayer()
  end
end

function Layer:print(prefix)
  if not prefix then
    prefix = ""
  end
  log.debug(prefix .."|-- ".. self.id)
  for k,view in pairs(self._subLayers) do
    view:print(prefix .."    ")
  end
end

function Layer:__tostring()
  return self:className() .."(".. self.id ..")"
end



module("lost.guiro.layer", package.seeall)

lost.common.Class "lost.guiro.layer.Layer" {}

function Layer:constructor()
	self._renderNode = lost.rg.Node.create()
	self._superlayer = nil
  self.sublayers = {}
	self.z = 0
	self.id = "layer"
end

function Layer:superlayer(...)
  if arg.n > 0 then
    local newsl = arg[1]
    self._superlayer = newsl
  else
    return self._superlayer
  end
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
  self:updateSublayerZ()
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



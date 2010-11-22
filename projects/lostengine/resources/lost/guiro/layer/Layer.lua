module("lost.guiro.layer", package.seeall)

lost.common.Class "lost.guiro.layer.Layer" {}

function Layer:constructor()
	self.renderNode = lost.rg.Node.create()
	self.superLayer = nil
  self.subLayers = {}
	self.z = 0
end

function Layer:addSublayer(layer)
end

function Layer:removeLayer(layer)
end

function Layer:removeFromSuperLayer()
end




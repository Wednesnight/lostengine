module("lost.guiro", package.seeall)

require("lost.common.Class")
require("lost.common.CallLater")

lost.common.Class "lost.guiro.UpdateManager" {}

function UpdateManager:constructor()
  self._updateScheduled = false
  -- each update queue has a set to track unique elements. within each set k == v, and if a new element was not present
  -- in the set it is also added to the list. The list is sorted by z values before the actual update funtions are called
  -- sets and lists are recreated after all updates have been performed
  self._layerUpdateQ = { set={}, list={}}
  self._layerLayoutQ = { set={}, list={}}
  self._layerDisplayQ = { set={}, list={}}
  self._viewUpdateQ = { set={}, list={}}
  self._viewLayoutQ = { set={}, list={}}
  self._viewDisplayQ = { set={}, list={}}
  
  self.lupdateFunc = function(layer) layer:update() end
  self.lupdateLayoutFunc = function(layer) layer:updateLayout() end
  self.lapplyLayoutfunc = function(layer) 
                                        if layer.layout then 
                                          layer.layout:apply(layer, layer.sublayers)
                                        end 
                                    end
  self.lupdateDisplayFunc = function(layer) layer:updateDisplay() end
  
  
  self.vupdateFunc = function(view) view:update() end
  self.vupdateLayoutFunc = function(view) view:updateLayout() end
  self.vapplyLayoutfunc = function(view)
                                     if view.layout then
                                       view.layout:apply(view, view.subviews)
                                     end
                                   end
  self.vupdateDisplayFunc = function(view) view:updateDisplay() end
end

local function depthSortFunc(a,b)
  return a.z < b.z
end

function UpdateManager:processQ(q, f, doSort, doClear)
--  log.debug("Q elems: "..tostring(#(q.list)))
  if doSort then table.sort(q.list, depthSortFunc) end
  for k,v in pairs(q.list) do
    f(v)
  end
  if doClear then
    q.list = {}
    q.set = {}
  end
end

function UpdateManager:processLayerUpdates()
  self:processQ(self._layerUpdateQ, self.lupdateFunc, true, true)
end

function UpdateManager:processLayerLayoutUpdates()
  self:processQ(self._layerLayoutQ, self.lupdateLayoutFunc, true, false)
  self:processQ(self._layerLayoutQ, self.lapplyLayoutfunc, false, true)
end

function UpdateManager:processLayerDisplayUpdates()
  self:processQ(self._layerDisplayQ, self.lupdateDisplayFunc, true, true)
end

function UpdateManager:processViewUpdates()
  self:processQ(self._viewUpdateQ, self.vupdateFunc, true, true)
end

function UpdateManager:processViewLayoutUpdates()
  self:processQ(self._viewLayoutQ, self.vupdateLayoutFunc, true, false)
  self:processQ(self._viewLayoutQ, self.vapplyLayoutfunc, false, true)
end

function UpdateManager:processViewDisplayUpdates()
  self:processQ(self._viewDisplayQ, self.vupdateDisplayFunc, true, true)
end

function UpdateManager:update()
--  log.debug("-- UPDATE")
  self:processViewUpdates()
  self:processViewLayoutUpdates()
  self:processViewDisplayUpdates()
  self:processLayerUpdates()
  self:processLayerLayoutUpdates()
  self:processLayerDisplayUpdates()
  self._updateScheduled = false
end

function UpdateManager:scheduleUpdateIfNeeded()
  if not self._updateScheduled then
--    log.debug("-- scheduling update")
    self._updateScheduled = true
    lost.common.callLater(UpdateManager.update, self)
  end
end

function UpdateManager:addElementToQ(q, e)
  if q.set[e] == nil then
    q.set[e] = e
    table.insert(q.list, e)
  end
end

function UpdateManager:layerNeedsUpdate(layer)
--  log.debug("layer needs update: ("..layer.z..") "..layer.id)
  self:addElementToQ(self._layerUpdateQ, layer)
  self:scheduleUpdateIfNeeded()
end

function UpdateManager:layerNeedsLayout(layer)
--  log.debug("layer needs layout: ("..layer.z..") "..layer.id)
  self:addElementToQ(self._layerLayoutQ, layer)
  self:scheduleUpdateIfNeeded()
end

function UpdateManager:layerNeedsDisplay(layer)
--  log.debug("layer needs display: ("..layer.z..") "..layer.id)
  self:addElementToQ(self._layerDisplayQ, layer)
  self:scheduleUpdateIfNeeded()
end

function UpdateManager:viewNeedsUpdate(view)
--  log.debug("view needs update: ("..view.z..") "..view.id)
  self:addElementToQ(self._viewUpdateQ, view)
  self:scheduleUpdateIfNeeded()
end

function UpdateManager:viewNeedsLayout(view)
--  log.debug("view needs layout: ("..view.z..") "..view.id)
  self:addElementToQ(self._viewLayoutQ, view)
  self:scheduleUpdateIfNeeded()
end

function UpdateManager:viewNeedsDisplay(view)
--  log.debug("view needs display: ("..view.z..") "..view.id)
  self:addElementToQ(self._viewDisplayQ, view)
  self:scheduleUpdateIfNeeded()
end

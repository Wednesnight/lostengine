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
  self._tq = {} -- temp queue for update loop
  self.lupdateFunc = function(layer) layer:update();lost.profiler.bb_inc("lost.guiro.UpdateManager.layerUpdate") end
  self.lupdateLayoutFunc = function(layer) layer:updateLayout();lost.profiler.bb_inc("lost.guiro.UpdateManager.layerUpdateLayout") end
  self.lapplyLayoutfunc = function(layer) 
                                        if layer.layout then 
                                          layer.layout:apply(layer, layer.sublayers)
                                          lost.profiler.bb_inc("lost.guiro.UpdateManager.layerApplyLayout")                                          
                                        end 
                                    end
  self.lupdateDisplayFunc = function(layer) layer:updateDisplay();lost.profiler.bb_inc("lost.guiro.UpdateManager.layerUpdateDisplay") end
  
  
  self.vupdateFunc = function(view) view:update();lost.profiler.bb_inc("lost.guiro.UpdateManager.viewUpdate") end
  self.vupdateLayoutFunc = function(view) 
--    log.debug("update layout view "..view.id)
    view:updateLayout();lost.profiler.bb_inc("lost.guiro.UpdateManager.viewUpdateLayout") 
  end
  self.vapplyLayoutfunc = function(view)
                                     if view.layout then
                                       view.layout:apply(view, view.subviews)
                                       lost.profiler.bb_inc("lost.guiro.UpdateManager.viewApplyLayout")                                                                                 
                                     end
                                   end
  self.vupdateDisplayFunc = function(view) view:updateDisplay();lost.profiler.bb_inc("lost.guiro.UpdateManager.layerUpdateDisplay") end
  lost.profiler.bb_set_clear("lost.guiro.UpdateManager.layerUpdate",true)
  lost.profiler.bb_set_clear("lost.guiro.UpdateManager.layerUpdateLayout",true)
  lost.profiler.bb_set_clear("lost.guiro.UpdateManager.layerApplyLayout",true)
  lost.profiler.bb_set_clear("lost.guiro.UpdateManager.layerUpdateDisplay",true)
  lost.profiler.bb_set_clear("lost.guiro.UpdateManager.viewUpdate",true)
  lost.profiler.bb_set_clear("lost.guiro.UpdateManager.viewUpdateLayout",true)
  lost.profiler.bb_set_clear("lost.guiro.UpdateManager.viewApplyLayout",true)
  lost.profiler.bb_set_clear("lost.guiro.UpdateManager.viewUpdateDisplay",true)
end

local function depthSortFunc(a,b)
  return a.z < b.z
end

function UpdateManager:processQ(q, f, doSort)
--  log.debug("Q elems: "..tostring(#(q.list)))
  local tempQ = {list=q.list,set=q.set}
  q.list={}
  q.set={}
  if doSort then table.sort(tempQ.list, depthSortFunc) end
  for k,v in pairs(tempQ.list) do
    f(v)
  end
end

function UpdateManager:processLayerUpdates()
    self:processQ(self._layerUpdateQ, self.lupdateFunc, true)
end

function UpdateManager:processLayerLayoutUpdates()
  local numUpdates = 0
  while next(self._layerLayoutQ.set) ~= nil do
--    log.debug("++ layer layout")
    self._tq.list = self._layerLayoutQ.list
    self._tq.set = self._layerLayoutQ.set    
    self:processQ(self._layerLayoutQ, self.lupdateLayoutFunc, true)
    self:processQ(self._tq, self.lapplyLayoutfunc, true)
    self:processQ(self._layerLayoutQ, self.lupdateLayoutFunc, true)
    numUpdates = numUpdates + 1
  end
--  log.debug("performed "..tostring(numUpdates).." layer layout updates")
end

function UpdateManager:processLayerDisplayUpdates()
  self:processQ(self._layerDisplayQ, self.lupdateDisplayFunc, true)
end

function UpdateManager:processViewUpdates()
  self:processQ(self._viewUpdateQ, self.vupdateFunc, true)
end

function UpdateManager:processViewLayoutUpdates()
  local numUpdates = 0
  while next(self._viewLayoutQ.set) ~= nil do
--    log.debug("++ view layout")
    self._tq.list = self._viewLayoutQ.list
    self._tq.set = self._viewLayoutQ.set        
    self:processQ(self._viewLayoutQ, self.vupdateLayoutFunc, true)
    self:processQ(self._tq, self.vapplyLayoutfunc, true)
    self:processQ(self._viewLayoutQ, self.vupdateLayoutFunc, true)
    numUpdates = numUpdates + 1
  end
--  log.debug("performed "..tostring(numUpdates).." view layout updates")
end

function UpdateManager:processViewDisplayUpdates()
  self:processQ(self._viewDisplayQ, self.vupdateDisplayFunc, true)
end

function UpdateManager:updateOnce()
--  log.debug("-- updateOnce")
  self:processViewUpdates()
  self:processViewLayoutUpdates()
  self:processViewDisplayUpdates()
  self:processLayerUpdates()
  self:processLayerLayoutUpdates()
  self:processLayerDisplayUpdates()
end

function UpdateManager:hasUpdates()
  local next = next
  return (next(self._viewUpdateQ.set)~=nil) or
         (next(self._viewLayoutQ.set)~=nil) or
         (next(self._viewDisplayQ.set)~=nil) or
         (next(self._layerUpdateQ.set)~=nil) or
         (next(self._layerLayoutQ.set)~=nil) or
         (next(self._layerDisplayQ.set)~=nil)
end

function UpdateManager:update()
--  log.debug("-- update")
  local numUpdates = 0
  while self:hasUpdates() do
    self:updateOnce()
    numUpdates = numUpdates + 1
    if numUpdates >= 3 then
--      log.warn("update cycle "..numUpdates)
    end
  end
  self._updateScheduled = false
--  log.debug("performed "..numUpdates.." update cycles")
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

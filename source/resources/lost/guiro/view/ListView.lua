module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.ListView" "lost.guiro.view.ScrollView" {}

local Color = lost.common.Color
local Vec2 = lost.math.Vec2

-- cells and headers must have a dataSource accessor function that allows setting/retrieving of the datasource table
-- as well as a reuseId string member that is used as a cache key
function ListView:constructor(args)
  lost.guiro.view.ScrollView.constructor(self, args)
  local t = args or {}
  self.id = t.id or "listview"
  self:hasHorizontalScrollbar(false)
  self:hasVerticalScrollbar(true)  
  self:addEventListener("contentPositionChanged",function(event) self:contentPositionChanged(event) end)
  
  if t.delegate ~= nil then self:delegate(t.delegate) end
  if t.dataSource ~= nil then self:dataSource(t.dataSource) end
  
  self._totalHeight = 0
  self._reloadInProgress = false
  self._geometry = {}
  
  self._cellCache = {active={},inactive={}}

  self:resetVisibleRange()
  self:reloadData()
end

function ListView:dumpStats()
  local numSubviews = 0
  for k,v in pairs(self.contentView.subviews) do
      numSubviews = numSubviews + 1
  end
  local activeCells = 0
  for reuseId,cache in pairs(self._cellCache.active) do
    for k,v in pairs(cache) do
      activeCells = activeCells + 1
    end
  end
  local inactiveCells = 0
  for reuseId,cache in pairs(self._cellCache.inactive) do
    for k,v in pairs(cache) do
      inactiveCells = inactiveCells + 1
    end
  end
  log.debug("-- contentView subviews: "..numSubviews)
  log.debug("-- cell cache active:"..activeCells.." inactive:"..inactiveCells)
end

function ListView:reconfigureIfNeeded()
  local vso = math.floor(self:calculateVerticalScrollOffset())
  local rtop,rbot = self:calculateVisibleRange(vso+self.rect.height-1,vso)
--  log.debug("checking "..rtop.." "..rbot.." VS "..self._rangeTop.." "..self._rangeBottom)
  if (rtop ~= self._rangeTop) or (rbot ~= self._rangeBottom) then
    self._prevRangeTop = self._rangeTop
    self._prevRangeBottom = self._rangeBottom
    self._rangeTop = rtop
    self._rangeBottom = rbot
--    log.debug("+++ RECONFIGURATION : vso "..vso.." rect height "..self.rect.height.." => "..rtop.."->"..rbot.." = "..((rbot-rtop)+1))
--    self:removeActiveSubviews()
--    self:deactivateAllCells()
    self:rebuildSubviews()
--    self:dumpStats()
  end
end

function ListView:resetVisibleRange()
  -- range of visible geometry elements
  self._prevRangeTop = 0
  self._prevRangeBottom = 0
  self._rangeTop = 0
  self._rangeBottom = 0
  self._cellAtIndex = {} -- contains the indices of the currently visible cells, for use with geometry table
end

function ListView:dequeueCell(reuseId)
  local result = nil
  if self._cellCache.inactive[reuseId] == nil then
--    log.debug("!!! creating new inactive cache for reuseId '"..reuseId.."'")
    self._cellCache.inactive[reuseId] = {}
  end
  if self._cellCache.active[reuseId] == nil then
--    log.debug("!!! creating new active cache for reuseId '"..reuseId.."'")
    self._cellCache.active[reuseId] = {}
  end
  local inactive = self._cellCache.inactive[reuseId]
--  self:dumpStats()
  for k,cell in pairs(inactive) do
    result = cell
    inactive[k]=nil
    break
  end
--  log.debug("!!! DEQUEUE for reuseId: '"..reuseId.."' "..tostring(result))
  return result
end

-- both headers and rows/cells
-- if active = true then cell is cached as an active cell, otherwise as an inactive one
function ListView:cacheCell(cell,active)
  if not cell.reuseId then
    error("cells and headers must have a reuseId string member")
  end
  local reuseId = cell.reuseId
  if self._cellCache.active[reuseId] == nil then
    self._cellCache.active[reuseId] = {}
  end  
  if self._cellCache.inactive[reuseId] == nil then
    self._cellCache.inactive[reuseId] = {}
  end  
  
  if active then
    self._cellCache.active[reuseId][cell] = cell
    self._cellCache.inactive[reuseId][cell] = nil
  else
    self._cellCache.active[reuseId][cell] = nil
    self._cellCache.inactive[reuseId][cell] = cell
  end
end

function ListView:deactivateAllCells()
  for reuseId,ridcache in pairs(self._cellCache.active) do
    for _,cell in pairs(ridcache) do
      self._cellCache.inactive[reuseId][cell] = cell
    end
    self._cellCache.active[reuseId] = {}
  end
--  log.debug("----------------------")
--  self:dumpStats()
--  log.debug("----------------------")
end

function ListView:clearCellCache()
--  log.debug("!!! CLEARING CELL CACHE !!!")
  self._cellCache.active = {}
  self._cellCache.inactive = {}
end

function ListView:removeActiveSubviews()
  for reuseId,cache in pairs(self._cellCache.active) do
    for c,cell in pairs(cache) do
      self.contentView:removeSubview(cell)
    end
  end  
end

-- top/bottom refer to pixel location, the order in which the cells are draw,top index value should always be < bottom
function _bsearch(data,top,bottom,val)
  while top <= bottom do
    local mid = top + math.floor((bottom - top) / 2)
    local cell = data[mid]
    if (val <= cell[1]) and (val > cell[2]) then
        return mid
    elseif val <= cell[2] then
        top = mid +1
    else
        bottom = mid -1
    end
  end
  return nil
end

function ListView:calculateVisibleRange(topPixelCoord, bottomPixelCoord)
  local top = _bsearch(self._geometry, 1,#self._geometry,topPixelCoord)
  if top == nil then top = 1 end
  local bottom = _bsearch(self._geometry, 1,#self._geometry,bottomPixelCoord)
  if bottom == nil then bottom = #self._geometry end
  return top, bottom
end

function ListView:contentPositionChanged(event)
  self:reconfigureIfNeeded()
end

function ListView:logGeometry()
  log.debug("--------")
  for k,v in ipairs(self._geometry) do
    local s = ""
    for _,val in ipairs(v) do
      s = s .. tostring(val).." "
    end
    log.debug(s)
  end
  log.debug("--------")
end

-- creates a table of tables containing the following values in this order
-- top global pixel coordinate in cell
-- bottom global pixel coordinate outside cell
-- 
function ListView:measureContent()
  local numSections = self:numberOfSections()
  self._totalContentHeight = 0
  self._geometry = {}
  for section=1,numSections do
    local numRows = self:numberOfRowsInSection(section)
    headerHeight = self:heightForHeaderInSection(section)
    self._totalContentHeight = self._totalContentHeight + headerHeight
    if headerHeight > 0 then
      table.insert(self._geometry,{0,headerHeight,true,section,0})
    end
    for row=1,numRows do
      local rowHeight = self:heightForRowAtIndexPath({section,row})
      self._totalContentHeight = self._totalContentHeight + rowHeight
      table.insert(self._geometry,{0,rowHeight,false,section,row})
    end
  end  
  -- second pass: correct the cell bounds, taking total height into account
  for i=1,#self._geometry do
    local cell = self._geometry[i]
    local prevcell = self._geometry[i-1]
    if prevcell ~= nil then
      cell[1] = prevcell[2]
      cell[2] = cell[1] - cell[2]
    else
      cell[1] = self._totalContentHeight-1
      cell[2] = cell[1] - cell[2]
    end
  end
--  self:logGeometry()
--  log.debug("total content height "..self._totalContentHeight)
end

function ListView:rebuildSubviews()
  local vy = self._totalContentHeight
  -- omit scan if this is the first run and no previous geometry was cached
  if not (self._prevRangeTop == 0) then -- if any of those indices is zero it's the first run
    -- scan previous range and deactivate and remove all cells that are not in the current one
    for i=self._prevRangeTop,self._prevRangeBottom do
      if not ((i>=self._rangeTop) and (i<=self._rangeBottom)) then
        local cell = self._cellAtIndex[i]
        self.contentView:removeSubview(cell)
        self:cacheCell(cell,false)
        self._cellAtIndex[i] = nil
--        log.debug("--- REMOVING "..i)
      end
    end
  end
  for i=self._rangeTop,self._rangeBottom do
    -- only create new cell if it wasn't already present in old range
    if not ((i>=self._prevRangeTop) and (i<=self._prevRangeBottom)) then 
--      log.debug("--- ADDING "..i)
      local entry = self._geometry[i]
      if entry[3] then -- header
        local section = entry[4]
        local header = self:viewForHeaderInSection(section)
        if not header then
          error(self.id.." returned nil header!")
        end
        header:x(0)
        header:y(entry[2]+1)
        header:width("1")
        header:height(entry[1]-entry[2])
        self.contentView:addSubview(header)
        local si = self:sectionInfoForIndex(section)
        header:dataSource(si)
        self:cacheCell(header,true)
        self._cellAtIndex[i]=header 
      else -- cell
        local ip = {entry[4],entry[5]}
        local cell = self:cellForRowAtIndexPath(ip)
        if not cell then
          error(self.id.." returned nil cell!")
        end
        cell:x(0)
        cell:y(entry[2]+1)
        cell:width("1")
        cell:height(entry[1]-entry[2])
        self.contentView:addSubview(cell)
        local ds = self:rowForIndexPath(ip)
        if cell.dataSource and (type(cell.dataSource)=="function") then
          cell:dataSource(ds)
        else
          error("cells must have a dataSource accessor method")
        end
        self:cacheCell(cell,true)
        self._cellAtIndex[i]=cell 
      end
    end
  end
end

function ListView:update()
  lost.guiro.view.ScrollView.update(self)
  if self._reloadInProgress then
    self:resetVisibleRange()
    self:removeActiveSubviews()
    self:clearCellCache()
    self:measureContent()
  end
end

function ListView:updateLayout()
  lost.guiro.view.ScrollView.updateLayout(self)  
  if self._reloadInProgress then
--    log.debug("RELOADING")
    self:contentSize(Vec2(self.rect.width,self._totalContentHeight))
    self:updateScrollbarVisibility()
    self:updateScrollbarVisibleRange()
    self:updateContentPosition()
    self._reloadInProgress = false
  end
  
  local w = "1"
  if not self.verticalScrollbar:hidden() then
    w = {"1",-self.scrollbarWidth}
  end
  self.contentView:width(w)

  self:reconfigureIfNeeded()
end

function ListView:reloadData()
  self._reloadInProgress = true
  self:needsUpdate()
  self:needsLayout()
end

function ListView:delegate(...)
  if arg.n >= 1 then
    self._delegate = arg[1]
    self:reloadData()
  else
    return self._delegate
  end
end

function ListView:dataSource(...)
  if arg.n >= 1 then
    self._dataSource = arg[1]
    self:reloadData()
  else
    return self._dataSource
  end
end

-- DataSource 
function ListView:numberOfSections()
  local result = 1
  if self._dataSource and self._dataSource.numberOfSections then
    result = self._dataSource:numberOfSections(self)
  end
  return result
end

function ListView:numberOfRowsInSection(sectionIndex)
  local result = 0
  if self._dataSource and self._dataSource.numberOfRowsInSection then
    result = self._dataSource:numberOfRowsInSection(self,sectionIndex)
  else
    error("'"..self.id.."' datasource and numberOfRowsInSection are mandatory")
  end
  return result
end

function ListView:sectionInfoForIndex(sectionIndex)
  local result = nil
  if self._dataSource and self._dataSource.sectionInfoForIndex then
    result = self._dataSource:sectionInfoForIndex(self,sectionIndex)
  end
  return result
end

function ListView:rowForIndexPath(indexPath)
  local result = 0
  if self._dataSource and self._dataSource.rowForIndexPath then
    result = self._dataSource:rowForIndexPath(self,indexPath)
  else
    error("'"..self.id.."' datasource and rowForIndexPath are mandatory")
  end
  return result
end

--Delegate

function ListView:viewForHeaderInSection(sectionIndex)
  local result = nil
  if self._delegate and self._delegate.viewForHeaderInSection then
    result = self._delegate:viewForHeaderInSection(self, sectionIndex)
  end
  return result
end

-- returning 0 means no header 
function ListView:heightForHeaderInSection(sectionIndex)
  local result = 0
  if self._delegate and self._delegate.heightForHeaderInSection then
    result = self._delegate:heightForHeaderInSection(sectionIndex)
  end
  return result
end

function ListView:cellForRowAtIndexPath(indexPath)
  local result = nil
  if self._delegate and self._delegate.cellForRowAtIndexPath then
    result = self._delegate:cellForRowAtIndexPath(self, indexPath)
  else
    if not self._delegate then
      error("'"..self.id.."' has no delegate")
    else
      error("'"..self.id.."' delegate has no cellForRowAtIndexPath method")
    end
  end
  return result
end

function ListView:heightForRowAtIndexPath(indexPath)
  local result = 40
  if self._delegate and self._delegate.heightForRowAtIndexPath then
    result = self._delegate:heightForRowAtIndexPath(self,indexPath)
  end
  return result
end

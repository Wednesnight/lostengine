module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.ListView" "lost.guiro.view.ScrollView" {}

local Color = lost.common.Color
local Vec2 = lost.math.Vec2

function ListView:constructor(args)
  lost.guiro.view.ScrollView.constructor(self, args)
  local t = args or {}
  self.id = t.id or "listview"
  self:hasHorizontalScrollbar(false)
  self:hasVerticalScrollbar(true)  
  
  if t.delegate ~= nil then self:delegate(t.delegate) end
  if t.dataSource ~= nil then self:dataSource(t.dataSource) end
  
  self._sectionHeights = {}
  self._totalHeight = 0
  self._reloadInProgress = false
  self._activeHeaders = {}
  self._activeCells = {}
  self._geometry = {}
  self:reloadData()
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

function ListView:measureContent()
  local numSections = self:numberOfSections()
  log.debug("---reloading, sections: "..numSections)
  self._totalContentHeight = 0
  self._geometry = {}
  for section=1,numSections do
    log.debug(section.." # "..self:numberOfRowsInSection(section))
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
  self:logGeometry()
  log.debug("total content height "..self._totalContentHeight)
end

function ListView:rebuildSubviews()
  for k,view in ipairs(self._activeHeaders) do
    self.contentView:removeSubview(view)
  end
  for k,cell in ipairs(self._activeCells) do
    self.contentView:removeSubview(cell)
  end  
  self._activeHeaders = {}
  self._activeCells = {}
  local vy = self._totalContentHeight
  for section=1,self:numberOfSections() do
    local header = self:viewForHeaderInSection(section)
    if header then
      vy = vy - self:heightForHeaderInSection(section)
--      log.debug("header at y "..vy)
      header:x(0)
      header:y(vy)
      header:width("1")
      header:height(self:heightForHeaderInSection())
      self.contentView:addSubview(header)
      local si = self:sectionInfoForIndex(section)
      header:dataSource(si)
      table.insert(self._activeHeaders,header)
    end
    local rows = self:numberOfRowsInSection(section)
    for row=1,rows do
      local ip = {section,row}
      local cell = self:cellForRowAtIndexPath(ip)
      local rowHeight = self:heightForRowAtIndexPath(ip)
      vy = vy - rowHeight
      cell:x(0)
      cell:y(vy)
      cell:width("1")
      cell:height(rowHeight)
      self.contentView:addSubview(cell)
      local ds = self:rowForIndexPath(ip)
      cell:dataSource(ds)
      table.insert(self._activeCells,cell)
    end
  end
end

function ListView:update()
  lost.guiro.view.ScrollView.update(self)
  if self._reloadInProgress then
    self:measureContent()
    self:rebuildSubviews()
  end
end

function ListView:updateLayout()
  lost.guiro.view.ScrollView.updateLayout(self)
  
  if self._reloadInProgress then
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
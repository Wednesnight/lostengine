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
end

function ListView:update()
  lost.guiro.view.ScrollView.update(self)
  
  local numSections = self:numberOfSections()
  log.debug("---reloading, sections: "..numSections)
  for i=1,numSections do
    log.debug(i.." # "..self:numberOfRowsInSection(i))
  end  
end

function ListView:reloadData()
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
    result = self._dataSource:numberOfSections()
  end
  return result
end

function ListView:numberOfRowsInSection(sectionIndex)
  local result = 0
  if self._dataSource and self._dataSource.numberOfRowsInSection then
    result = self._dataSource:numberOfRowsInSection(sectionIndex)
  else
    error("'"..self.id.."' datasource and numberOfRowsInSection are mandatory")
  end
  return result
end

function ListView:sectionInfoForIndex(sectionIndex)
  local result = nil
  if self._dataSource and self._dataSource.sectionInfoForIndex then
    result = self._dataSource:sectionInfoForIndex(sectionIndex)
  end
  return result
end

function ListView:rowForIndexPath(indexPath)
  local result = 0
  if self._dataSource and self._dataSource.rowForIndexPath then
    result = self._dataSource:rowForIndexPath(indexPath)
  else
    error("'"..self.id.."' datasource and rowForIndexPath are mandatory")
  end
  return result
end

--Delegate

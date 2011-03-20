require("lost.common.Class")

lost.common.Class "FileListDatasource" {}

function FileListDatasource:constructor(data)
  log.debug("constructor")
  self._data = data
end

function FileListDatasource:numberOfRowsInSection(listView,section)
  local result = 0
  for k,v in pairs(self._data) do
    result = result+1
  end
  return result
end

function FileListDatasource:rowForIndexPath(listView,indexPath)
  return self._data[indexPath[2]]
end
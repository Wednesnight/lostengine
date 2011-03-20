require("lost.common.Class")

lost.common.Class "FileListDatasource" {}

function FileListDatasource:constructor()
  log.debug("constructor")
end

function FileListDatasource:numberOfRowsInSection(listView,section)
  return 0
end
require("lost.common.Class")

lost.common.Class "MainController" {}

function MainController:constructor()
  log.debug("maincontroller constructor")
  self.files = {}
  self.fileListDelegate = FileListDelegate()
  self.fileListDatasource = FileListDatasource(self.files)
end

function MainController:buildUi()
  lost.guiro.ui():add{
    require("ui")
  }
  self.fileList = lost.guiro.ui():recursiveFindById("fileList")
  self.fileList:delegate(self.fileListDelegate)
  self.fileList:dataSource(self.fileListDatasource)
end

function MainController:fileDropped(path)
  log.debug(path)
  local entry = {["path"]=path}
  table.insert(self.files,entry)
  self.fileList:reloadData()
end


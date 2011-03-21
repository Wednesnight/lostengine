require("lost.common.Class")
require("FileListDatasource")
require("FileListDelegate")

lost.common.Class "MainController" {}

function MainController:constructor()
  log.debug("maincontroller constructor")
  self.files = {}
  self.fileListDelegate = FileListDelegate()
  self.fileListDatasource = FileListDatasource(self.files)
  self.rootLoader = lost.resource.Loader.create()
  self.rootLoader:addRepository(lost.resource.FilesystemRepository.create("/"))
  self.gifDecoder = lost.bitmap.GifDecoder(self.rootLoader)
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

  local p = boost.filesystem.path(path)

  if not boost.filesystem.is_regular_file(p) then 
    log.debug("ERROR: dropped item was not a file, ignoring!")
    return 
  end
  
  local entry = 
  {
      ["path"]=path,
      filename = tostring(p:stem()),
      extension = tostring(p:extension()),
      isGif = self.gifDecoder:isGif(path),
  }
  
  if entry.isGif then
    entry.gif = self.gifDecoder:load(path)
    entry.bitmap = entry.gif:bitmap(0)
    entry.numBitmaps = entry.gif:numBitmaps()
  else
    local data = self.rootLoader:load(path)
    local ok=true
    ok,entry.bitmap = pcall(lost.bitmap.Bitmap.create,data)
    if ok then
      entry.bitmap:flip()
      entry.bitmap:premultiplyAlpha()
    else
      log.error("couldn't create Bitmap object from data at '"..tostring(path).."'")
      return
    end
  end
  local texparams = lost.gl.Texture.Params()
  entry.texture = lost.gl.Texture.create(entry.bitmap,texparams)
  
  table.insert(self.files,entry)
  self.fileList:reloadData()
end


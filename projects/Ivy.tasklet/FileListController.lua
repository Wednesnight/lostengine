require("lost.common.Class")
require("FileListCell")

lost.common.Class "FileListController" {}

function FileListController:constructor()
  self.files = {}
  self.rootLoader = lost.resource.Loader.create()
  self.rootLoader:addRepository(lost.resource.FilesystemRepository.create("/"))
  self.gifDecoder = lost.bitmap.GifDecoder(self.rootLoader)  
end

function FileListController:loadView()
  self._view = require("ui_files")

  self.fileList = self._view:recursiveFindById("fileList")
  self.fileEmpty = self._view:recursiveFindById("filesEmpty")
  self.filePreview = self._view:recursiveFindById("filesPreview")
  self.fileList:delegate(self)
  self.fileList:dataSource(self)  
end

function FileListController:view(...)
  if arg.n >= 1 then
    if self._view then
      self._view:removeFromSuperview()
    end
    self._view = arg[1]
  else
    if not self._view then
      self:loadView()
    end
    return self._view
  end
end

function FileListController:numLoadedFiles()
  local result =0
  for k,v in pairs(self.files) do
    result = result + 1
  end
  return result
end

function FileListController:fileAtIndex(idx)
  return self.files[idx]
end

function FileListController:numberOfRowsInSection(listView,section)
  return self:numLoadedFiles()
end

function FileListController:rowForIndexPath(listView,indexPath)
  return self:fileAtIndex(indexPath[2])
end

function FileListController:heightForRowAtIndexPath(listView,indexPath)
  return 50
end

function FileListController:cellForRowAtIndexPath(listView,indexPath)
  local result = listView:dequeueCell(FileListCell.reuseId)
  if not result then
    result = FileListCell{}
  end
  result:switchColor((indexPath[2] % 2)==0)
  result:select(self:fileAtIndex(indexPath[2]).selected)
  result.delegate = self
  return result
end

function FileListController:updateViewVisibility()
  if self:numLoadedFiles() == 0 then
    self.fileEmpty:show()
    self.filePreview:hide()
  else
    self.fileEmpty:hide()
    self.filePreview:show()
  end
end

function FileListController:addFile(path)
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
  self:updateViewVisibility()
end
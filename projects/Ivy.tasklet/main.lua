require("MainController")
require("FileListDatasource")
require("FileListDelegate")

local rootLoader = lost.resource.Loader.create()
rootLoader:addRepository(lost.resource.FilesystemRepository.create("/"))
local gifDecoder = lost.bitmap.GifDecoder(rootLoader)

local Color = lost.common.Color

function startup()
  require("lost/guiro")  
  mainController = MainController()
  mainController:buildUi()
end


function update()
end

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end
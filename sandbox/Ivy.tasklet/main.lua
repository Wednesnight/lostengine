-- Ivy main.lua
require("lost.declarative.Context")
require("sizes")

dcl = nil
rootLoader = lost.resource.Loader.create()
rootLoader:addRepository(lost.resource.FilesystemRepository.create("/"))

function keyHandler(event)
  if event.key == lost.application.K_ESCAPE then
    running = false
  end
end

function startup()
  running = true
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)
  
  dcl = lost.declarative.Context(tasklet.loader)
  screen = require("ui")

  imageView = screen:recursiveFindById("imageView")
  pathNameLabel = screen:recursiveFindById("pathNameLabel")
  helpLabel = screen:recursiveFindById("helpLabel")
  sizePixelsLabel = screen:recursiveFindById("sizePixelsLabel")
  sizeBytesLabel = screen:recursiveFindById("sizeBytesLabel")
  log.debug("imageView: "..tostring(imageView))

  tasklet.eventDispatcher:addEventListener(lost.application.DragNDropEvent.DROP, dropHandler)
  
  return true
end

function update()
  return running
end

function loadHelper(path)
  log.debug("trying to load from path: "..path)
  local data = rootLoader:load(path)
  local bmp = lost.bitmap.Bitmap.create(data)
  return bmp, data
end

function dropHandler(dropEvent)
  local path = dropEvent.filename
  log.debug("dropped file: '"..path.."'")
  
  -- result is either an error description or the created bitmap object
  local status, result, d = pcall(function() return loadHelper(path) end)
  if status then
    imageView:bitmap(result)
    pathNameLabel:text(path)
    helpLabel:hidden(true)
    sizePixelsLabel:text(tostring(result.width).." x "..tostring(result.height).." Pixels")
    sizeBytesLabel:text(tostring(d.size).." Bytes")
  else
    log.error("Couldn't create bitmap from data, reason: "..tostring(result))
  end
end

-- Ivy main.lua
require("lost.declarative.Context")

local Bounds = lost.guiro.Bounds
local xabs = lost.guiro.xabs
local yabs = lost.guiro.yabs
local wabs = lost.guiro.wabs
local habs = lost.guiro.habs
local wrel = lost.guiro.wrel
local hrel = lost.guiro.hrel

require("sizes")

windowParams = lost.application.WindowParams("Ivy", lost.math.Rect(200,200,640,480))
dcl = nil
rootLoader = lost.resource.Loader.create()
rootLoader:addRepository(lost.resource.FilesystemRepository.create("/"))

function keyHandler(event)
  if event.key == lost.application.K_ESCAPE then
    running = false
  end
end

function startup(tasklet)
  tasklet.name = "Ivy"
  tasklet.waitForEvents = true
  dcl = lost.declarative.Context(tasklet.loader)
  screen = require("ui")
  screen.bounds = Bounds(xabs(0), yabs(0), wabs(windowParams.rect.width), habs(windowParams.rect.height))
  screen:listenTo(tasklet.eventDispatcher)
  callLater(function() screen:updateLayout(true) end) 
  running = true
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)
  
  imageView = screen:recursiveFindById("imageView")
  pathNameLabel = screen:recursiveFindById("pathNameLabel")
  helpLabel = screen:recursiveFindById("helpLabel")
  sizePixelsLabel = screen:recursiveFindById("sizePixelsLabel")
  sizeBytesLabel = screen:recursiveFindById("sizeBytesLabel")
  log.debug("imageView: "..tostring(imageView))

  tasklet.eventDispatcher:addEventListener(lost.application.DropEvent.DROPPED_FILE, dropHandler)
  
  return true
end

function update(tasklet)
--  log.debug("update")
  processCallLaterQueue()
  screen:updateLayout(false)
  screen.rootNode:process(tasklet.window.context)
  tasklet.window.context:swapBuffers()
  return running
end

function shutdown(tasklet)
  return true
end

function loadHelper(path)
  log.debug("trying to oad from path: "..path)
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
    imageView:needsLayout()
    pathNameLabel:text(path)
    helpLabel:hidden(true)
    screen:updateLayout(false)
    sizePixelsLabel:text(tostring(result.width).." x "..tostring(result.height).." Pixels")
    sizeBytesLabel:text(tostring(d.size).." Bytes")
  else
    log.error("Couldn't create bitmap from data, reason: "..tostring(result))
  end
end

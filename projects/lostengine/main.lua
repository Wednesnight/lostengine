require("lost.declarative.Context")

local WindowParams = lost.application.WindowParams
local Rect = lost.math.Rect
local KeyEvent = lost.application.KeyEvent
local ApplicationEvent = lost.application.ApplicationEvent
local DropEvent = lost.application.DropEvent
local K_ESCAPE = lost.application.K_ESCAPE
local Loader = lost.resource.Loader
local FilesystemRepository = lost.resource.FilesystemRepository
local ApplicationResourceRepository = lost.resource.ApplicationResourceRepository
local SpawnTaskletEvent = lost.application.SpawnTaskletEvent

lostengineTasklet = nil
windowParams = WindowParams("LostEngine",
                            Rect(50,800,400, 128))

function receive(type, handler)
  return function(event) handler(type.cast(event)) end
end

function startup(tasklet)

  lostengineTasklet = tasklet
  tasklet.waitForEvents = true

  tasklet.eventDispatcher:addEventListener(KeyEvent.KEY_DOWN, receive(KeyEvent, keyDownHandler))
  tasklet.eventDispatcher:addEventListener(DropEvent.DROPPED_FILE, receive(DropEvent, fileDropHandler))

  -- load UI
  dcl = lost.declarative.Context(tasklet.loader)
  ui = require("ui")

  return true
end

function update(tasklet)
  ui:process(tasklet.window.context)
  tasklet.window.context:swapBuffers()
  return true
end

function shutdown(tasklet)
  return true
end

function keyDownHandler(event)
  if event.key == K_ESCAPE then
    lostengineTasklet:dispatchApplicationEvent(ApplicationEvent(ApplicationEvent.QUIT))
  end
end

function fileDropHandler(event)
  log.debug("starting tasklet: ".. event.filename)
  local loader = Loader()
  loader:addRepository(FilesystemRepository.create(event.filename))
  loader:addRepository(ApplicationResourceRepository.create())
  lostengineTasklet:dispatchApplicationEvent(SpawnTaskletEvent(loader))
end

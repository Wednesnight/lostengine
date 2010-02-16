require("lost.declarative.Context")

using "lost.application.WindowParams"
using "lost.math.Rect"
using "lost.application.ApplicationEvent"
using "lost.declarative.Context"


--[[
    setup windowParams
    this should be done in startup, fix this after implementing Window:setParams()
  ]]
windowParams = WindowParams("LostEngine", Rect(50,800,400,128))


--[[
    tasklet startup
  ]]
function startup(tasklet)

  -- setup tasklet
  tasklet.name = "LostEngine Launcher"
  tasklet.waitForEvents = true

  dcl = Context(tasklet.loader)
  local screen = require("ui")

  -- debug
  tasklet.renderNode:print()
  screen:printSubviews()

  -- finally, setup events
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)

  return true
end


--[[
    key handler
  ]]
function keyHandler(event)
  local tasklet = lost.application.currentTasklet
  if event.key == lost.application.K_ESCAPE and tasklet ~= nil then
    tasklet:dispatchApplicationEvent(ApplicationEvent.create(ApplicationEvent.QUIT))
  end
end

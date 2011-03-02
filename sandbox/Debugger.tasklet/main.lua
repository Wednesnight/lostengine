local application = lost.application.Application.getInstance()

local mainView = nil
local labelWidth = 200
local labelHeight = 15

local eventProxies = {}

local refreshTimer = nil

function startup()
  require("lost.guiro")

  local padding = 15
  mainView = lost.guiro.view.View
  {
    bounds = {"left", "bottom", "1", {"1", -labelHeight*2}}
  }
  lost.guiro.ui():add{
    lost.guiro.view.View
    {
      bounds = {{"left", padding}, {"top", -padding}, {"1", -padding*2}, {"1", -padding*2}},
      subviews =
      {
        lost.guiro.view.Label
        {
          bounds = {"left", "top", labelWidth, labelHeight*2},
          font = {"Vera", 14},
          halign = "left",
          valign = "center",
          text = "Running tasklets:"
        },
        mainView,
        lost.guiro.view.Button
        {
          bounds = {"right", "bottom", 100, labelHeight*2},
          title = "refresh now",
          listeners =
          {
            buttonClick = function(event)
              refresh()
            end
          }
        }
      }
    }
  }

  tasklet.eventDispatcher:addEventListener(lost.application.DebugEvent.MEM_INFO, function(event)
    mainView(tostring(event.tasklet.id))("memInfo"):text("memSize: ".. tostring(event.info.memSize))
  end)

  refreshTimer = tasklet.scheduler:createTimer(2, refresh, nil)
  refreshTimer:start()

  refresh()
end

function refresh(t)
  mainView:removeAllSubviews()

  local offset = 0
  for t in application.tasklets do
    if t.running then
      local name = t.name
      if t.id == tasklet.id then
        name = "[".. name .."]"
      end
      mainView:addSubview(lost.guiro.view.View
      {
        id = tostring(t.id),
        subviews =
        {
          lost.guiro.view.Label
          {
            id = "name",
            bounds = {{"left", 10}, {"top", -offset}, labelWidth, labelHeight},
            font = {"Vera", 10},
            halign = "left",
            valign = "center",
            text = name
          },
          lost.guiro.view.Label
          {
            id = "memInfo",
            bounds = {{"left", 20}, {"top", -(offset+labelHeight)}, labelWidth, labelHeight},
            font = {"Vera", 10},
            halign = "left",
            valign = "center",
            text = ""
          }
        }
      })
      offset = offset + labelHeight*2

      -- listen to debug events
      for t in application.tasklets do
        if t.id ~= tasklet.id and eventProxies[t.eventDispatcher] == nil then
          eventProxies[t.eventDispatcher] = tasklet.eventDispatcher:attachTo(t.eventDispatcher, lost.application.DebugEvent.MEM_INFO)
        end
      end

      -- request memory info
      local request = lost.application.DebugEvent.create(lost.application.DebugEvent.GET_MEM_INFO, t)
      t.eventDispatcher:queueEvent(request)
    end
  end

  return true
end

function shutdown()
  refreshTimer:stop()

  for dispatcher,connection in next,eventProxies do
    dispatcher:removeEventListener(connection)
  end
end

function key(event)
  log.debug("key")
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end

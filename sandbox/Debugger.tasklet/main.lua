local application = lost.application.Application.getInstance()

local mainView = nil
local labelWidth = 120
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

  local offset = 0
  for t in application.tasklets do
    if t.running then
      offset = addTaskletInfo(t, offset)
    end
  end

  eventProxies[application.eventDispatcher] =
  {
    tasklet.eventDispatcher:attachTo(application.eventDispatcher, lost.application.TaskletEvent.START),
    tasklet.eventDispatcher:attachTo(application.eventDispatcher, lost.application.TaskletEvent.DONE)
  }

  tasklet.eventDispatcher:addEventListener(lost.application.TaskletEvent.START, function(event)
    offset = addTaskletInfo(event.tasklet, offset)
  end)

  tasklet.eventDispatcher:addEventListener(lost.application.TaskletEvent.DONE, function(event)
    local current = {}
    for k,view in next,mainView.subviews do
      current[view.id] = view
    end
    for t in application.tasklets do
      if t.running then
        current[tostring(t.id)] = nil
      end
    end
    for id,view in next,current do
      offset = removeTaskletInfo(view)
    end
  end)

  tasklet.eventDispatcher:addEventListener(lost.application.DebugEvent.MEM_INFO, function(event)
    mainView(tostring(event.tasklet.id))("memInfo"):text("memSize: ".. tostring(event.info.memSize))
  end)

  tasklet.eventDispatcher:addEventListener(lost.application.DebugEvent.PAUSE, function(event)
    mainView(tostring(event.tasklet.id))("button"):title("continue")
    mainView(tostring(event.tasklet.id))("button"):enabled(true)
  end)

  refreshTimer = tasklet.scheduler:createTimer(2, function(t)
    for t in application.tasklets do
      if t.running then
        t.eventDispatcher:queueEvent(lost.application.DebugEvent.create(lost.application.DebugEvent.CMD_MEM_INFO, t))
      end
    end
    return true
  end, nil)
  refreshTimer:start()
end

function addTaskletInfo(t, offset)
  local name = t.name
  if t.id == tasklet.id then
    name = "[".. name .."]"
  end
  mainView:addSubview(lost.guiro.view.View
  {
    id = tostring(t.id),
    bounds = {{"left", 10}, {"top", -offset}, labelWidth+85, labelHeight*2},
    subviews =
    {
      lost.guiro.view.Label
      {
        id = "name",
        bounds = {"left", "top", labelWidth, labelHeight},
        font = {"Vera", 10},
        halign = "left",
        valign = "center",
        text = name
      },
      lost.guiro.view.Button
      {
        id = "button",
        bounds = {{"left", labelWidth+10}, "top", 75, labelHeight},
        title = "pause",
        hidden = (t.id == tasklet.id),
        listeners =
        {
          buttonClick = function(event)
            if event.target:title() == "pause" then
              t.eventDispatcher:queueEvent(lost.application.DebugEvent.create(lost.application.DebugEvent.CMD_PAUSE, t))
              event.target:enabled(false)
            else
              t.eventDispatcher:queueEvent(lost.application.DebugEvent.create(lost.application.DebugEvent.CMD_CONTINUE, t))
              event.target:title("pause")
            end
          end
        }
      },
      lost.guiro.view.Label
      {
        id = "memInfo",
        bounds = {{"left", 10}, {"top", -labelHeight}, labelWidth, labelHeight},
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
      eventProxies[t.eventDispatcher] = 
      {
        tasklet.eventDispatcher:attachTo(t.eventDispatcher, lost.application.DebugEvent.MEM_INFO),
        tasklet.eventDispatcher:attachTo(t.eventDispatcher, lost.application.DebugEvent.PAUSE)
      }
    end
  end

  -- request memory info
  t.eventDispatcher:queueEvent(lost.application.DebugEvent.create(lost.application.DebugEvent.CMD_MEM_INFO, t))

  return offset
end

function removeTaskletInfo(infoView)
  mainView:removeSubview(infoView)
  local offset = 0
  for k,view in next,mainView.subviews do
    view:bounds(lost.guiro.Bounds({"left", 10}, {"top", -offset}, labelWidth+85, labelHeight*2))
    offset = offset + labelHeight*2
  end
  return offset
end

function shutdown()
  refreshTimer:stop()

  for dispatcher,connections in next,eventProxies do
    for _,connection in next,connections do
      dispatcher:removeEventListener(connection)
    end
  end
  eventProxies = {}
end

function key(event)
  log.debug("key")
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end

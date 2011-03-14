local application = lost.application.Application.getInstance()

local mainView = nil
local summaryView = nil
local tabPadding = 25
local labelWidth = 120
local labelHeight = 15

local eventProxies = {}

local refreshTimer = nil

function startup()
  require("lost.guiro")

  summaryView = lost.guiro.view.View
  {
    bounds = {"left", "bottom", "1", {"1", -labelHeight*2}}
  }
  mainView = lost.guiro.view.TabView
  {
    bounds = {"left", "top", "1", "1"},
    style = "square",
    items =
    {
      {
        "Summary",
        lost.guiro.view.View
        {
          id = "summary",
          bounds = {{"left", tabPadding}, {"top", -tabPadding}, {"1", -tabPadding*2}, {"1", -tabPadding*2}},
          subviews =
          {
            lost.guiro.view.Label
            {
              bounds = {"center", "top", labelWidth, labelHeight*2},
              font = {"Vera", 14},
              halign = "left",
              valign = "center",
              text = "Running tasklets:"
            },
            summaryView,
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
    }
  }
  lost.guiro.ui():add{
    mainView
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
    for k,view in next,summaryView.subviews do
      current[view.id] = view
    end
    for t in application.tasklets do
      if t.running then
        current[tostring(t.id)] = nil
      end
    end
    for id,view in next,current do
      offset = removeTaskletInfo(view)
      mainView:removeItem(id)
    end
  end)

  tasklet.eventDispatcher:addEventListener(lost.application.DebugEvent.MEM_INFO, function(event)
    summaryView(tostring(event.tasklet.id))("memInfo"):text("memSize: ".. tostring(event.info.memSize))
  end)

  tasklet.eventDispatcher:addEventListener(lost.application.DebugEvent.PAUSE, function(event)
    local info = event.tasklet.lua:getScriptSource(event.info.debug) ..", line ".. event.info.debug.currentline
    mainView(tostring(event.tasklet.id))("info"):text(info)
    mainView(tostring(event.tasklet.id))("button"):title("continue")
    mainView(tostring(event.tasklet.id))("button"):enabled(true)
    mainView(tostring(event.tasklet.id))("step"):hidden(false)
  end)

  refreshTimer = tasklet.scheduler:createTimer(2, function(t)
    refresh()
    return true
  end, nil)
  refreshTimer:start()
end

function refresh()
  for t in application.tasklets do
    if t.running then
      t.eventDispatcher:queueEvent(lost.application.DebugEvent.create(lost.application.DebugEvent.CMD_MEM_INFO, t))
    end
  end
end

function addTaskletInfo(t, offset)
  local name = t.name
  if t.id == tasklet.id then
    name = "[".. name .."]"
  end
  summaryView:addSubview(lost.guiro.view.View
  {
    id = tostring(t.id),
    bounds = {"center", {"top", -offset}, labelWidth+60, labelHeight*2},
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
        bounds = {{"left", labelWidth+10}, "top", 50, labelHeight},
        title = "-->",
        hidden = (t.id == tasklet.id),
        listeners =
        {
          buttonClick = function(event)
            local id = tostring(t.id)
            if not mainView:select(id) then
              mainView:addItem(t.name, lost.guiro.view.View {
                id = id,
                bounds = {{"left", tabPadding}, {"top", -tabPadding}, {"1", -tabPadding*2}, {"1", -tabPadding*2}},
                subviews =
                {
                  lost.guiro.view.Button
                  {
                    id = "button",
                    bounds = {"left", "top", 75, labelHeight},
                    title = "pause",
                    listeners =
                    {
                      buttonClick = function(event)
                        if event.target:title() == "pause" then
                          t.eventDispatcher:queueEvent(lost.application.DebugEvent.create(lost.application.DebugEvent.CMD_PAUSE, t))
                          event.target:enabled(false)
                        else
                          t.eventDispatcher:queueEvent(lost.application.DebugEvent.create(lost.application.DebugEvent.CMD_CONTINUE, t))
                          event.target:title("pause")
                          event.target:superview()("step"):hidden(true)
                        end
                      end
                    }
                  },
                  lost.guiro.view.Button
                  {
                    id = "step",
                    bounds = {{"left", 80}, "top", 75, labelHeight},
                    title = "step",
                    hidden = true,
                    listeners =
                    {
                      buttonClick = function(event)
                        local event = lost.application.DebugEvent.create(lost.application.DebugEvent.CMD_CONTINUE, t)
                        event.mode = 1
                        t.eventDispatcher:queueEvent(event)
                      end
                    }
                  },
                  lost.guiro.view.Label
                  {
                    id = "info",
                    bounds = {"left", {"top", -labelHeight}, "1", {"1", -labelHeight}},
                    font = {"Vera", 10},
                    halign = "left",
                    valign = "top",
                    text = ""
                  }
                }
              })
              mainView:select(id)
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
  summaryView:removeSubview(infoView)
  local offset = 0
  for k,view in next,summaryView.subviews do
    view:bounds(lost.guiro.Bounds("center", {"top", -offset}, labelWidth+60, labelHeight*2))
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

lost.application.Application =
{
  addEventListener = function(self, which, where)
    globals.app:addEventListener(which, where)
  end,

  addEventListenerMethod = function(self, which, owner, where)
    globals.app:addEventListener(which, owner, where)
  end,

  config = {},

  context = globals.app.context
}

function fireEvent(name, event)
  if type(lost.application.Application[name]) == "function" then
    lost.application.Application[name](lost.application.Application, event)
  end
end

function eventHandler(event)
  if (event.type == lost.application.ApplicationEvent.PREINIT) then
    fireEvent("configure", event)
  elseif (event.type == lost.application.ApplicationEvent.INIT) then
    fireEvent("init", event)
  elseif (event.type == lost.application.ApplicationEvent.RUN) then
    fireEvent("run", event)
  elseif (event.type == lost.application.ApplicationEvent.QUIT) then
    fireEvent("quit", event)
  end
end

globals.app:addEventListener(lost.application.ApplicationEvent.PREINIT, eventHandler)
globals.app:addEventListener(lost.application.ApplicationEvent.INIT,    eventHandler)
globals.app:addEventListener(lost.application.ApplicationEvent.RUN,     eventHandler)
globals.app:addEventListener(lost.application.ApplicationEvent.QUIT,    eventHandler)

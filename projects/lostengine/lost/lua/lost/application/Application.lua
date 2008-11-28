lost.application.Application = lost.globals.app

function lost.application.Application:fireEvent(name, event)
  if type(self[name]) == "function" then
    self[name](self, event)
  end
end

function lost.application.Application:eventHandler(event)
  if (event.type == lost.application.ApplicationEvent.PREINIT) then
    self:fireEvent("configure", event)
  elseif (event.type == lost.application.ApplicationEvent.INIT) then
    self:fireEvent("init", event)
  elseif (event.type == lost.application.ApplicationEvent.RUN) then
    self:fireEvent("run", event)
  elseif (event.type == lost.application.ApplicationEvent.QUIT) then
    self:fireEvent("exit", event)
  end
end

lost.globals.app:addEventListener(lost.application.ApplicationEvent.PREINIT, function(event) lost.application.Application:eventHandler(event) end)
lost.globals.app:addEventListener(lost.application.ApplicationEvent.INIT,    function(event) lost.application.Application:eventHandler(event) end)
lost.globals.app:addEventListener(lost.application.ApplicationEvent.RUN,     function(event) lost.application.Application:eventHandler(event) end)
lost.globals.app:addEventListener(lost.application.ApplicationEvent.QUIT,    function(event) lost.application.Application:eventHandler(event) end)

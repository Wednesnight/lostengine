Application =
{
  addEventListener = function(self, which, where)
    globals.app:addEventListener(which, where)
  end,

  config = {}
}

function preinitHandler(event)
  if type(Application.configure) == "function" then
    Application:configure(event)
  end
end

function initHandler(event)
  if type(Application.init) == "function" then
    Application:init(event)
  end
end

function runHandler(event)
  if type(Application.run) == "function" then
    Application:run(event)
  end
end

function quitHandler(event)
  if type(Application.quit) == "function" then
    Application:quit(event)
  end
end

globals.app:addEventListener(lost.application.ApplicationEvent.PREINIT, preinitHandler)
globals.app:addEventListener(lost.application.ApplicationEvent.INIT, initHandler)
globals.app:addEventListener(lost.application.ApplicationEvent.RUN, runHandler)
globals.app:addEventListener(lost.application.ApplicationEvent.QUIT, quitHandler)

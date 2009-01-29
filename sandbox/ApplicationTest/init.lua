--local window = lost.globals.app:createWindow("window", 
--                 lost.application.WindowParams("Application", lost.math.Vec2(800, 600), lost.math.Vec2(100, 100)))
lost.globals.app:setRunLoop(function() log.debug("this is a lua runLoop") end)


local Color = lost.common.Color
local Vec2 = lost.math.Vec2

function startup()  
  require("lost.guiro")
  require("MainViewController")
  local r = lost.guiro.layer.Rect
  local rr = lost.guiro.layer.RoundedRect
  local t = lost.guiro.layer.Text
  
  mainViewController = MainViewController()  
  lost.guiro.ui():addSubview(mainViewController.view)
end

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end

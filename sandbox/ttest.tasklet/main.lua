
local Color=lost.common.Color
local Vec2=lost.math.Vec2

function startup()
  require("lost.guiro")

  lost.guiro.ui():add
  {
    lost.guiro.view.Label
    {
      bounds={0,"top",200,20},
      text= "hello",
    },
    lost.guiro.view.TextInput
    {
      bounds={50,50,200,30},
    }
  }
end

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end

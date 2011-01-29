
local Color=lost.common.Color
local Vec2=lost.math.Vec2

local f = {"Vera",12}

function startup()
  require("lost.guiro")

  lost.guiro.ui():add
  {
    lost.guiro.view.Label
    {
      bounds={0,"top",200,20},
      text= "hello",
      font=f
    },
    lost.guiro.view.TextInput
    {
      listeners=
      {
        valueChanged=function(event) log.debug("'"..event.target:text().."'") end,
        enterPressed=function(event) log.debug(event.type) end,
      },
      
      bounds={50,50,200,30},
      halign="left",
      font=f,
      text="yeah",
      halign = "center"
    }
  }
end

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end

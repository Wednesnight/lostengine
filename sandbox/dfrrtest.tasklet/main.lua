local Color = lost.common.Color

function startup()
  require("lost/guiro")
  lost.guiro.ui():add
  {
    lost.guiro.view.Label
    {
      text="Hello"
    }
  }
  
  local c1 = Color(0,0,0)
  local c2 = Color(1,0,0)
  local c3 = Color(1,1,1)
  local b = {50,50,100,100}
  lost.guiro.ui().layer:addSublayer(lost.guiro.layer.RoundedRect{bounds=b,filled=true,radius=r,color=c1})
  lost.guiro.ui().layer:addSublayer(lost.guiro.layer.dfrr{bounds=b,filled=true,radius=r,color=c2})
end

function update()
end

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end
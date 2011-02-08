local Color = lost.common.Color


function sliderInputLabelView(viewBounds, labelText, sliderMin, sliderMax, updateFunc)
  return lost.guiro.view.View
  {
    bounds = viewBounds,
    listeners =
    {
      valueChanged = function(event)
                        local v = 0
                        if event.target.id == "input" then
                          v = tonumber(event.target:text()) or 0
                          event.currentTarget("slider"):value(v)
                        else
                          v = math.floor(event.target:value())
                          event.currentTarget("input"):text(tostring(v))
                        end
                        updateFunc(v)
                      end
    },
    layout=lost.guiro.layout.Horizontal{valign="center",spacing=4},
    subviews=
    {
      lost.guiro.view.Label
      {
        text=labelText,
        bounds={0,"center",".15",20}
      },
      lost.guiro.view.TextInput
      {
        id="input",
        bounds={0, "center", ".13", 20},
        halign="center",
      },
      lost.guiro.view.Slider
      {
        id="slider",
        bounds={0, "center", ".75", 20},
        min=sliderMin,
        max=sliderMax,
      },
    }
  }
end

function startup()
  require("lost/guiro")

  local c1 = Color(0,0,0)
  local c2 = Color(1,0,0)
  local c3 = Color(1,1,1)
  local b = {50,50,{".5",-100},{"1",-100}}

  local l = lost.guiro.layer.dfrr{bounds=b,filled=true,radius=r,color=c2}
  lost.guiro.ui():add
  {
    lost.guiro.view.View
    {
      bounds={"right",0,".5","1"},
      layout=lost.guiro.layout.Vertical{halign="center",valign="center",spacing=10},
      subviews={
        sliderInputLabelView({0,0,300, 30}, "Width:", 0, 100, function(val) l.mesh.material.uniforms:setFloat("width", val) end),
        sliderInputLabelView({0,0,300, 30}, "Radius:", 0, 100, function(val) l.mesh.material.uniforms:setFloat("radius", val) end)        
      }
    }
  }
  
  lost.guiro.ui().layer:addSublayer(lost.guiro.layer.RoundedRect{bounds=b,filled=true,radius=r,color=c1})
  lost.guiro.ui().layer:addSublayer(l)
end

function update()
end

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end
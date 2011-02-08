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
        bounds={0,"center",".12",20}
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

function colorView(viewBounds, labelText, updateFunc)
  return lost.guiro.view.View
  {
    bounds = viewBounds,  
    layout=lost.guiro.layout.Horizontal{valign="center",spacing=4},
    listeners=
    {
      valueChanged=function(event)
          updateFunc(event.currentTarget("color"):color())
        end
    },
    subviews=
    {
      lost.guiro.view.Label
      {
        bounds={0,"center", ".4", "1"},
        text=labelText,
        halign="left",
      },
      lost.guiro.view.ColorPicker
      {
        id="color",
        name=labelText,
        bounds={0, "center", ".6", "1"}
      }
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
  local bg = lost.guiro.layer.Rect{bounds=b,filled=true,color=c1}
  lost.guiro.ui():add
  {
    lost.guiro.view.View
    {
      id="main",
      bounds={{"right",-10},0,".5","1"},
      layout=lost.guiro.layout.Vertical{halign="center",valign="center",spacing=10},
      subviews={
        lost.guiro.view.Button{
          bounds={0,0,"1",30},title="Filled",style="checkboxCandy",
          size="regular",
          pushed=true,
          listeners=
          {
            buttonClick=function(event)
              l:filled(event.target:pushed())
            end
          }
        },
        lost.guiro.view.Box
        {
          id="corners",
          title="Round corners",
          bounds={0,0,"1",50},
          subviews=
          {
            lost.guiro.view.Button{bounds={"left","bottom", ".5",".5"},id="bl",style="checkboxCandy",title="bottom left"},
            lost.guiro.view.Button{bounds={"right","bottom", ".5",".5"},id="br",style="checkboxCandy",title="bottom right"},
            lost.guiro.view.Button{bounds={"left","top", ".5",".5"},id="tl",style="checkboxCandy",title="top left"},
            lost.guiro.view.Button{bounds={"right","top", ".5",".5"},id="tr",style="checkboxCandy",title="top right"},
          },
          listeners=
          {
            buttonClick=function(event) 
              if event.target.id == "bl" then
                l:blround(event.target:pushed())
              elseif event.target.id == "br" then
                l:brround(event.target:pushed())
              elseif event.target.id == "tr" then
                l:trround(event.target:pushed())
              elseif event.target.id == "tl" then
                l:tlround(event.target:pushed())
              end
            end
          }
        },
        lost.guiro.view.Button{
          bounds={0,0,"1",30},title="Show Background",style="checkboxCandy",
          size="regular",
          pushed=true,
          listeners=
          {
            buttonClick=function(event)
              bg:hidden(not event.target:pushed())
            end
          }
        },
        colorView({0,0,"1", 30}, "Background Color:", function(val) bg:color(val) end),
        colorView({0,0,"1", 30}, "Rect Color:", function(val) l:color(val) end),
        sliderInputLabelView({0,0,"1", 30}, "Width:", 0, 100, function(val) 
            if l.mesh.material.shader:hasUniform("width") then
              l:width(val) 
            end
          end),
        sliderInputLabelView({0,0,"1", 30}, "Radius:", 0, 100, function(val) 
            if l.mesh.material.shader:hasUniform("radius") then
              l:radius(val) 
            end
          end)        
      }
    }
  }
  
--  lost.guiro.ui():printSubviews()
  lost.guiro.ui().layer:addSublayer(bg)
  lost.guiro.ui().layer:addSublayer(l)
  lost.guiro.ui()("main")("corners")("view")("bl"):pushed(l:blround())
  lost.guiro.ui()("main")("corners")("view")("br"):pushed(l:brround())
  lost.guiro.ui()("main")("corners")("view")("tl"):pushed(l:tlround())
  lost.guiro.ui()("main")("corners")("view")("tr"):pushed(l:trround())
end

function update()
end

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end
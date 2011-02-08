local Color = lost.common.Color


function startup()
  require("lost/guiro")

  local c1 = Color(0,0,0)
  local c2 = Color(1,0,0)
  local c3 = Color(1,1,1)
  local b = {50,50,{".5",-100},{"1",-100}}

  local l = lost.guiro.layer.dfrr{bounds=b,filled=true,radius=r,color=c2}
  lost.guiro.ui():add
  {
      lost.guiro.view.Slider
      {
        id="slider",
        listeners=
        {
          valueChanged = function(event) 
            local v = math.floor(event.target:value())
            log.debug(v) 
            l.mesh.material.uniforms:setFloat("width", v)
            lost.guiro.ui()("input"):text(tostring(v))
          end
        },
        bounds={{"right",-10}, {"top",-50}, 200, 20},
        min=0,
        max=100,
      },
      lost.guiro.view.TextInput
      {
        id="input",
        bounds={{"right",-250}, {"top",-50}, 50, 20},
        halign="center",
        listeners =
        {
          valueChanged = function(event) 
                          local v = tonumber(event.target:text()) or 0
                          log.debug(v)
                          l.mesh.material.uniforms:setFloat("width", v)
                          lost.guiro.ui()("slider"):value(v)                          
                        end
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
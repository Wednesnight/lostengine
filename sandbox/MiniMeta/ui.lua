return lost.guiro.view.View
{
  id="controlPanel",
  bounds={"right",0,200,"1"},
  style="gray",
  listeners={valueChanged = function(event) 
        tasklet.window.context:shader(metaShader)
        if event.target.id == "drama" then
          metaShader:setFloat("drama",event.target:value())
        elseif event.target.id == "ambience" then
          metaShader:setFloat("ambience",event.target:value())
        end
      end},
  subviews=
  {
    lost.guiro.view.Slider
    {
      id="drama",
      size="mini",
      bounds={"center",{"top",-20},{"1",-40},20},
      min=1,
      max=200,
    },
    lost.guiro.view.Slider
    {
      id="ambience",
      bounds={"center",{"top",-60},{"1",-40},20},
      min=0,
      max=1,
    },
    lost.guiro.view.PopUpButton
    {
      id="presets",
      size="mini",
      bounds={"center",{"top",-100},{"1",-40},20},
    }
  }
}
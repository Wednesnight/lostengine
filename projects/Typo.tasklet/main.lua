local Color = lost.common.Color

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end

function startup()
  require("lost/guiro")

  local fnt = tasklet.fontManager:getFont("Vera", 12)

  lost.guiro.ui():add{
    lost.guiro.view.View
    {
      subviews={
        lost.guiro.view.TextInput
        {
          bounds={10,{"top",-10},200,20},
          font={"Vera", 12},
        },
        lost.guiro.view.View
        {
          bounds={{"right",-10},10,fnt.atlasSize.width,fnt.atlasSize.height},
          sublayers=
          {
            lost.guiro.layer.Rect{color=Color(0,0,0),filled=true}
          }
        },
        lost.guiro.view.Image
        {
          bounds={{"right",-10},10,fnt.atlasSize.width,fnt.atlasSize.height},
          texture=fnt.atlas,
          scale="none",
          filter=false,
          flip=false
        }
      }
    }
  }
end
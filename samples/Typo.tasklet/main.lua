local Color = lost.common.Color

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end

local m = 10

function startup()
  require("lost/guiro")

  local fnt = tasklet.fontManager:getFont("Vera", 12)

  lost.guiro.ui():add{
    lost.guiro.view.View
    {
      style="gray",
      subviews={
        lost.guiro.view.TextInput
        {
          bounds={m,{"top",-m},200,2*m},
          font={"Vera", 12},
        },
        lost.guiro.view.View
        {
          bounds={{"right",-m},m,fnt.atlasSize.width,fnt.atlasSize.height},
          sublayers=
          {
            lost.guiro.layer.Rect{color=Color(0,0,0),filled=true}
          }
        },
        lost.guiro.view.Image
        {
          bounds={{"right",-m},m,fnt.atlasSize.width,fnt.atlasSize.height},
          texture=fnt.atlas,
          scale="none",
          filter=false,
          flip=false
        }
      }
    }
  }
end
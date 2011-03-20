local rootLoader = lost.resource.Loader.create()
rootLoader:addRepository(lost.resource.FilesystemRepository.create("/"))
local gifDecoder = lost.bitmap.GifDecoder(rootLoader)

local Color = lost.common.Color

function startup()
  require("lost/guiro")
  paletteView = lost.guiro.view.Image
  {
    bounds={10,{"top",-10},256,20},
    scale="none",
    valign="center",
    halign="center"
  }

  bitmapView = lost.guiro.view.Image
  {
    bounds={10,10,300,300},
    scale="none",
    valign="center",
    halign="center"
  }
  
  lost.guiro.ui():add{
    lost.guiro.view.View
    {
      listeners=
      {
        drop=function(event)
          log.debug(event.filename)
          local isGif = gifDecoder:isGif(event.filename)
          log.debug("is gif: "..tostring(isGif))
          if isGif then
            gif = gifDecoder:load(event.filename)
            log.debug("loaded ok")
            log.debug("numBitmaps: "..gif:numBitmaps())
            paletteView:bitmap(gif:paletteAsBitmap())
            bitmapView:bitmap(gif:bitmap(0))
          end
        end
      },
      subviews=
      {
        paletteView,
        bitmapView
      }
    }
  }
end


function update()
end

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end
local rootLoader = lost.resource.Loader.create()
rootLoader:addRepository(lost.resource.FilesystemRepository.create("/"))
local gifDecoder = lost.bitmap.GifDecoder(rootLoader)

local Color = lost.common.Color

function startup()
  require("lost/guiro")
  imageView = lost.guiro.view.Image
  {
    bounds={10,10,600,200},
    scale="aspect",
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
            log.debug("numImages: "..gif:numImages())
            local paletteBitmap = gif:paletteAsBitmap()
            local b = lost.bitmap.Bitmap.create(200,200,lost.bitmap.COMPONENTS_RGBA)
            b:clear(Color(1,0,0))
            imageView:bitmap(paletteBitmap)
          end
        end
      },
      subviews=
      {
        imageView
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
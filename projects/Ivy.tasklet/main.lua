local rootLoader = lost.resource.Loader.create()
rootLoader:addRepository(lost.resource.FilesystemRepository.create("/"))
local gifDecoder = lost.bitmap.GifDecoder(rootLoader)

function startup()
  require("lost/guiro")
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
          end
        end
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
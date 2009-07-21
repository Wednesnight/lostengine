require("lost.io.Rg")
require("settings")

hasWindow = true
windowParams = lost.application.WindowParams("MeshTest", lost.math.Rect(50,200,screenSize.width, screenSize.height))

function startup(tasklet)

  local ttflib = lost.font.freetype.Library.create()
  verattf = lost.font.TrueTypeFont.create(ttflib, tasklet.loader:load("Vera.ttf"))
  verattf.atlasSize = lost.math.Vec2(128,64);
  local l = lost.io.Loader(tasklet.loader)
  scene = l:loadScene("testscene.lua")
  threedScene = l:loadScene("3dScene.lua")
  textScene = l:loadScene("textScene.lua")
  tunaScene = l:loadScene("tunaScene.lua")
  pixelperfectScene = l:loadScene("pixelperfect.lua")
  
  tasklet.eventDispatcher:addEventListener(lost.application.DropEvent.DROPPED_FILE,
    function(event)
      local dropEvent = lost.application.DropEvent.cast(event)
      log.debug(dropEvent.filename)
    end
  )
  
  return true
end

require("lost.io.Rg")

function startup(tasklet)

  local ttflib = lost.font.freetype.Library.create()
  verattf = lost.font.TrueTypeFont.create(ttflib, tasklet.loader:load("Vera.ttf"))

  local l = lost.io.Loader(tasklet.loader)
  scene = l:loadScene("testscene.lua")

  return true
end

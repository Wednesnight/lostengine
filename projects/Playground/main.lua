require("lost.io.Rg")
require("settings")

function startup(tasklet)
  local ttflib = lost.font.freetype.Library.create()
  verattf = lost.font.TrueTypeFont.create(ttflib, tasklet.loader:load("Vera.ttf"))
  verattf.atlasSize = lost.math.Vec2(128,64);
  local l = lost.io.Loader(tasklet.loader)
  textScene = l:loadScene("textScene.lua")
  return true
end

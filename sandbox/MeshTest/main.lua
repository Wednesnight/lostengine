require("lost.declarative.Context")
require("settings")

local WindowParams = lost.application.WindowParams
local Rect = lost.math.Rect

windowParams = WindowParams("MeshTest", Rect(50,200,screenSize.width, screenSize.height))

function startup(tasklet)

  local ttflib = lost.font.freetype.Library.create()
  verattf = lost.font.TrueTypeFont.create(ttflib, tasklet.loader:load("Vera.ttf"))
  verattf.atlasSize = lost.math.Vec2(128,64);

  dcl = lost.declarative.Context(tasklet.loader)

  scene = require("testscene")
  threedScene = require("3dScene")
  textScene = require("textScene")
  tunaScene = require("tunaScene")
  pixelperfectScene = require("pixelperfect")
  
  return true
end

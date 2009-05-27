require("lost.io.Rg")

function startup(tasklet)
  local l = lost.io.Loader(tasklet.loader)
  scene = l:loadScene("testscene.lua")
  return true
end

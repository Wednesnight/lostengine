require("lost.io.Rg")


windowParams = lost.application.WindowParams("fuxx0r", lost.math.Rect(50,200,640,480))

angle = 0
lastUpdate = 0

function startup(tasklet)
  log.debug("startup")
	local l = lost.io.Loader(tasklet.loader)
  threedScene = l:loadScene("3dScene.lua")
  local drawCube = lost.rg.Draw.cast(threedScene:recursiveFindByName("cube"))
  cubeMesh = drawCube.mesh
	lastUpdate = lost.platform.currentTimeMilliSeconds()
  return true
end

function update(tasklet)
	local currentTime = lost.platform.currentTimeMilliSeconds() 
	local dt = currentTime - lastUpdate
  angle = math.fmod(dt*.05+angle, 360);
  cubeMesh.transform = lost.math.MatrixRotX(angle) * lost.math.MatrixRotY(angle);
  threedScene:process(tasklet.window.context)
	tasklet.window.context:swapBuffers()
	lastUpdate = currentTime
  return true
end

function shutdown(tasklet)
  log.debug("shutdown")
  return true
end

require("lost.common.Class")

lost.common.Class "Animator" {}

--[[
Helper class for animation of enities
has got two modes
1.) real time updates: each call to update calculates new delta and absolute running time
    and calls update with these values on the given entity
2.) dump mode: calls update with a fixed frequency and writes screenshots to a given directory
]]
function Animator:constructor()
  self.lastUpdate = 0
  self.startTime = 0
end

function Animator:reset()
  self.startTime = lost.platform.currentTimeMilliSeconds()
  self.lastUpdate = self.startTime
end

function Animator:update(tasklet, entity)
  local currentTime = lost.platform.currentTimeMilliSeconds()
  local dt = currentTime - self.lastUpdate -- delta time since last update
  local at = currentTime - self.startTime -- absolute time since last reset
  entity:update(tasklet, dt, at)
  entity.rootNode:process(tasklet.window.context)
  tasklet.window.context:swapBuffers()      
  self.lastUpdate = currentTime
end

-- screenshots are written as "XXX.tga" with XXX being replaced with the current 
-- frame number, NO leading zeros
function Animator:dump(tasklet, entity, fps, totalTimeMsec, screenshotDir)
  local stepDelta = 1000 / fps
  local numFrames = totalTimeMsec / stepDelta
  local filename = nil
  local at = 0 
  local dt = 0
  for i=1,numFrames do
    entity:update(tasklet, dt, at)
    at = at + stepDelta
    dt = stepDelta
    entity.rootNode:process(tasklet.window.context)
    tasklet.window.context:swapBuffers()      
    filename = screenshotDir..i..".tga" 
    log.debug(filename)
    tasklet.window.context:writeScreenshot(filename, false)
  end
end
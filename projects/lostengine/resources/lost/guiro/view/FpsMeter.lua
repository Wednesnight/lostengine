module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.FpsMeter" "lost.guiro.view.View" {}

-- mandatory style parameters
-- * textLayer: will receive the current number of fps for the last measurement
-- optional:
-- *avg: number of frames whose fps will be averaged to the value that is actually displayed
function FpsMeter:constructor(args)
	lost.guiro.view.View.constructor(self, args) -- call after all members that are required for style are setup
  self.id = args.id or "fpsmeter"  
  self.last = lost.platform.currentTimeSeconds()
  self.num = 0
  self.sum = 0
  self.avg = args.avg or 10
  self.textLayer:text("0") -- must be set before first render or we'll crash
  self:updatefps()
end

function FpsMeter:updatefps()
--  log.debug("fpsmeter update")
  local now = lost.platform.currentTimeSeconds()
  local delta = now - self.last
  self.last = now
  self.sum = self.sum + delta
  self.num = self.num +1
  if self.num >= self.avg then
    local fps = self.num / self.sum
    local txt = string.format("%3.0f",fps)
    self.textLayer:text(txt)    
    self.num = 0
    self.sum = 0
  end
  lost.common.callLater(self.updatefps,self)
end

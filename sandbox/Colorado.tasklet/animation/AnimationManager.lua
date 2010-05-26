require("lost.common.Class")

lost.common.Class "AnimationManager" {}

function AnimationManager:constructor()
  self.animations = {}
end

function AnimationManager:add(animation)
  table.insert(self.animations, animation)
end

function AnimationManager:remove(animation)
  for k,a in next,self.animations do
    if a == animation then
      table.remove(self.animations, k)
      break
    end
  end
end

function AnimationManager:process()
  if self.currentTime == nil then
    self.currentTime = lost.platform.currentTimeSeconds()
  end
  local delta = lost.platform.currentTimeSeconds() - self.currentTime
  for idx = #self.animations,1,-1 do
    if not self.animations[idx]:process(delta) then
      table.remove(self.animations,idx)
    end
  end
  return #self.animations > 0
end

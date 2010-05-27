require("lost.common.Class")

lost.common.Class "AnimationManager" {}

function AnimationManager:constructor(loader, renderNode, board)
  self.animations = {}
  self.board = board
  self.loader = loader
  self.renderNode = renderNode

  self.ftlib = lost.font.freetype.Library.create()
  self.defaultFont = lost.font.TrueTypeFont.create(self.ftlib, self.loader:load("lost/resources/ttf-bitstream-vera-1.10/Vera.ttf"))
  self.defaultFont.atlasSize = lost.math.Vec2(256, 256)
end

function AnimationManager:add(animation)
  table.insert(self.animations, animation)
  animation:init(self)
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
  local currentTime = lost.platform.currentTimeSeconds()
  local delta = currentTime - self.currentTime
  self.currentTime = currentTime
  for idx = #self.animations,1,-1 do
    if not self.animations[idx]:process(delta) then
      table.remove(self.animations,idx)
    end
  end
  return #self.animations > 0
end

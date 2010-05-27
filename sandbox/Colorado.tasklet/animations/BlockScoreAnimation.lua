require("animations.BlockAnimation")
require("lost.common.Shaders")

lost.common.Class "BlockScoreAnimation" "BlockAnimation" {}

function BlockScoreAnimation:constructor(blockSize, boardSize, boardPos, blockCoords, score)
  BlockAnimation.constructor(self, blockCoords)
  self.blockSize = blockSize
  self.boardSize = boardSize
  self.boardPos = boardPos
  self.score = score
  self.size = 1
end

function BlockScoreAnimation:init(manager)
  BlockAnimation.init(self, manager)

  self.textMesh = lost.font.RenderedText.create()
  self.textMesh.material.shader = lost.common.Shaders.textureShader()
  self.textMesh.material.color = lost.common.Color(1,1,1,1)
  self.manager.defaultFont:render(tostring(self.score), 8, self.textMesh)
  self.textNode = lost.rg.Draw.create(self.textMesh)
  self.manager.renderNode:add(self.textNode)
end

function BlockScoreAnimation:process(deltaSec)
  self.size = self.size + self.size * deltaSec
  if self.size <= 3 then
    self.textMesh.transform = lost.math.MatrixTranslation(
      lost.math.Vec3(self.boardPos.x + self.blockSize.x/2 + self.blockSize.x * (self.coords.x-1),
                     self.boardPos.y + self.blockSize.y/2 + self.blockSize.y * (self.coords.y-1), 0)) *
        lost.math.MatrixScaling(lost.math.Vec3(self.size, self.size, 0))
    return true
  else
    self.manager.renderNode:remove(self.textNode)
    return false
  end
end

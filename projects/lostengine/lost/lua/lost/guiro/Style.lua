module("lost.guiro", package.seeall) -- Style

require("lost.common.Class")

lost.common.Class "lost.guiro.Style" {}

-- each derived style MUST set name and className so themes and managers can associate a style
-- with a possible target
function Style:constructor()
  self.name = ""
  self.targetClassName = ""
end

-- called if an external entity found that this style can be applied to target
-- style should then set the appropriate properties on the target.
function Style:apply(target)
end

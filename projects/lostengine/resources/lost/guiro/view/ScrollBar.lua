module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.ScrollBar" "lost.guiro.view.View" {}

-- * handleSize: [0..1] in percent of the current grooveSize
-- * handleLayer: must be inside grooveLayer
-- * grooveLayer: background of handle, contains handle
-- * upButton
-- * downButton
function ScrollBar:constructor(args)
  lost.guiro.view.View.constructor(self, args)
  local t = args or {}
  self.id = t.id or "scrollbar"
end

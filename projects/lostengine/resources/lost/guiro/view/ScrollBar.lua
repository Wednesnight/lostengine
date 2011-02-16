module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.ScrollBar" "lost.guiro.view.Slider" {}

-- * handleSize: [0..1] in percent of the current grooveSize
-- * handleLayer: must be inside grooveLayer
-- * grooveLayer: background of handle, contains handle
-- * upButton
-- * downButton
function ScrollBar:constructor(args)
  lost.guiro.view.Slider.constructor(self, args)
  local t = args or {}
  self.id = t.id or "scrollbar"
end

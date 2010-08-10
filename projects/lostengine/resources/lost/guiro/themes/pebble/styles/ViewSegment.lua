-- lost.guiro.themes.pebble.styles.ViewSegment
module("lost.guiro.themes.pebble.styles", package.seeall)

require("lost.common.Class")
require("lost.guiro.Style")

using("lost.common.Color")

lost.common.Class "lost.guiro.themes.pebble.styles.ViewSegment" "lost.guiro.Style" {}

function ViewSegment:constructor()
  lost.guiro.Style(self)
  self.name = "segment"
  self.targetClassName = "lost.guiro.view.View"
  self.cornerRadius = 4
  self.height = {}
  self.height["regular"] = 21;
  self.height["small"] = 18;
  self.height["mini"] = 15;
end

function ViewSegment:apply(target, def)
  local height = 21
  if def and def.size then
    height = self.height[def.size] or height
  end
  
  local orientation = "mid"
  if def and def.orientation then
    orientation = def.orientation
  end
  
  local segcolor = "grey"
  if def and def.segmentColor then
    segcolor = def.segmentColor
  end
  
  target.bounds.height = lost.guiro.hsize{abs=height}
  target:backgroundCornerRadius(self.cornerRadius)
  target:frameCornerRadius(self.cornerRadius)
  target:showFrame(true)
  target:showBackground(true)
  target:backgroundColor(Color(1,1,1))
  target:frameColor(Color(1, 1, 1))
  
  if segcolor == "grey" then
    target:backgroundGradient("segLightGrey")
    target:frameGradient("segGreyFrame")
  else
    target:backgroundGradient("segBlue")
    target:frameGradient("segBlueFrame")
  end

  if orientation == "left" then
    target:frameRoundCorners(true, true, true, true)
    target:backgroundRoundCorners(true, false, true, false)
    target:frameShowSides(true, true, true, false)
  elseif orientation == "mid" then
    target:frameRoundCorners(false, true, false, true)
    target:backgroundRoundCorners(false, false, false, false)
    target:frameShowSides(true, true, true, false)
  elseif orientation == "right" then
    target:frameRoundCorners(false, true, false, true)
    target:backgroundRoundCorners(false, true, false, true)
    target:frameShowSides(true, true, true, true)
  end

  target:needsRedraw()
  target:needsLayout()
end

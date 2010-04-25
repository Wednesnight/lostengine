-- lost.guiro.themes.pebble.buildGradients
module("lost.guiro.themes.pebble", package.seeall) 

using "lost.common.ColorGradient"
using "lost.common.ColorPoint"
using "lost.common.Color"

-- interprets val as a byte value, builds a grey color from it
function bgrey(val)
  local c = val/255
  return Color(c, c, c)
end

function buildGradients(textureManager)
  local rrbg = ColorGradient.create()
  local rrbg2 = ColorGradient.create()

  local c1 = 222/255
  local c2 = 245/255
  rrbg:add(ColorPoint(0, bgrey(222)))
  rrbg:add(ColorPoint(1, bgrey(245)))

--  if not self.textureManager:hasGradient("rrbg") then
    textureManager:addGradient("rrbg", rrbg)
--  end

  c1 = 210/255
  c2 = 167/255
  rrbg2:add(ColorPoint(0, bgrey(210)))
  rrbg2:add(ColorPoint(1, bgrey(167)))

--  if not self.textureManager:hasGradient("rrbg2") then
    textureManager:addGradient("rrbg2", rrbg2)
--  end
end
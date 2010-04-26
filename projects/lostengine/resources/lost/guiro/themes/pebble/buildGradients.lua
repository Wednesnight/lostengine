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

-- interprets cx value as byte ranging from 0-255, returns Color object with normalized values
-- c4 is optional and set to 1 if omitted
function bcol(c1, c2, c3, c4)
  local result = nil
  if c4 ~= nil then
    result = Color(c1/255, c2/255, c3/255, c4/255)
  else
    result = Color(c1/255, c2/255, c3/255, 1)
  end
  return result
end

function roundRectGradients(textureManager)
  local rrbg = ColorGradient.create()
  local rrbg2 = ColorGradient.create()

  rrbg:add(ColorPoint(0, bgrey(222)))
  rrbg:add(ColorPoint(1, bgrey(245)))
  textureManager:addGradient("rrbg", rrbg)

  rrbg2:add(ColorPoint(0, bgrey(210)))
  rrbg2:add(ColorPoint(1, bgrey(167)))
  textureManager:addGradient("rrbg2", rrbg2)  
end

function bubbleGradients(textureManager)
  local bubbleGreyFrame = ColorGradient.create()
  bubbleGreyFrame:add(ColorPoint(1, bgrey(109)))
  bubbleGreyFrame:add(ColorPoint(0, bgrey(92)))
  textureManager:addGradient("bubbleGreyFrame", bubbleGreyFrame)  


  local bubbleGreyBg = ColorGradient.create()
  bubbleGreyBg:add(ColorPoint(12/12, bgrey(253)))
  bubbleGreyBg:add(ColorPoint(11/12, bgrey(255)))
  bubbleGreyBg:add(ColorPoint(10/12, bgrey(252)))
  bubbleGreyBg:add(ColorPoint(9/12, bgrey(245)))
  bubbleGreyBg:add(ColorPoint(8/12, bgrey(239)))
  bubbleGreyBg:add(ColorPoint(7/12, bgrey(233)))
  bubbleGreyBg:add(ColorPoint(6/12, bgrey(236)))
  bubbleGreyBg:add(ColorPoint(5/12, bgrey(241)))
  bubbleGreyBg:add(ColorPoint(4/12, bgrey(249)))
  bubbleGreyBg:add(ColorPoint(3/12, bgrey(255)))
  bubbleGreyBg:add(ColorPoint(2/12, bgrey(255)))
  bubbleGreyBg:add(ColorPoint(1/12, bgrey(255)))
  bubbleGreyBg:add(ColorPoint(0/12, bgrey(241)))
  textureManager:addGradient("bubbleGreyBg", bubbleGreyBg)  

end

function buildGradients(textureManager)
  roundRectGradients(textureManager)
  bubbleGradients(textureManager)
end
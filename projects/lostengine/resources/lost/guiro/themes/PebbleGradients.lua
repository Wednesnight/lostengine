local ColorGradient = lost.common.ColorGradient
local ColorPoint = lost.common.ColorPoint
local Color = lost.common.Color

-- interprets val as a byte value, builds a grey color from it
local function bgrey(val)
  local c = val/255
  return Color(c, c, c)
end

-- interprets cx value as byte ranging from 0-255, returns Color object with normalized values
-- c4 is optional and set to 1 if omitted
local function bcol(c1, c2, c3, c4)
  local result = nil
  if c4 ~= nil then
    result = Color(c1/255, c2/255, c3/255, c4/255)
  else
    result = Color(c1/255, c2/255, c3/255, 1)
  end
  return result
end

local function miscGradients(textureManager)
  local cg = ColorGradient.create()

  cg:add(ColorPoint(1, bgrey(208)))
  cg:add(ColorPoint(0, bgrey(167)))
  textureManager:addGradient("toolbarBg", cg)  

  cg = ColorGradient.create()
  cg:add(ColorPoint(1, Color(1,1,1)))
  cg:add(ColorPoint(.7, Color(1,.4,.4)))
  cg:add(ColorPoint(0, Color(1,0,0)))
  textureManager:addGradient("closeButtonFill", cg)  

  cg = ColorGradient.create()
  cg:add(ColorPoint(1, Color(.1,0,0)))
  cg:add(ColorPoint(0, Color(1,0,0)))
  textureManager:addGradient("closeButtonFrame", cg)  

  cg = ColorGradient.create()
  cg:add(ColorPoint(1, bgrey(171)))
  cg:add(ColorPoint(.99, bgrey(197)))
  cg:add(ColorPoint(.98, bgrey(218)))
  cg:add(ColorPoint(.97, bgrey(228)))
  cg:add(ColorPoint(0, bgrey(229)))
  textureManager:addGradient("recess", cg)  

  cg = ColorGradient.create()
  cg:add(ColorPoint(21/21, bgrey(251)))
  cg:add(ColorPoint(20/21, bgrey(242)))
  cg:add(ColorPoint(19/21, bgrey(229)))
  cg:add(ColorPoint(0/21, bgrey(249)))
  textureManager:addGradient("colorpicker", cg)  

end

local function roundRectGradients(textureManager)
  local rrbg = ColorGradient.create()
  local rrbg2 = ColorGradient.create()

  rrbg:add(ColorPoint(0, bgrey(222)))
  rrbg:add(ColorPoint(1, bgrey(245)))
  textureManager:addGradient("rrbg", rrbg)

  rrbg2:add(ColorPoint(0, bgrey(210)))
  rrbg2:add(ColorPoint(1, bgrey(167)))
  textureManager:addGradient("rrbg2", rrbg2)  
end

local function bubbleGradients(textureManager)
  local bubbleGreyFrame = ColorGradient.create()
  bubbleGreyFrame:add(ColorPoint(1, bgrey(109)))
  bubbleGreyFrame:add(ColorPoint(0, bgrey(92)))
  textureManager:addGradient("bubbleGrayFrame", bubbleGreyFrame)  

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
  textureManager:addGradient("bubbleGrayBg", bubbleGreyBg)  

  local bubbleBlueBg = ColorGradient.create()
  bubbleBlueBg:add(ColorPoint(12/12, bcol(239,243,249)))
  bubbleBlueBg:add(ColorPoint(11/12, bcol(225,236,247)))
  bubbleBlueBg:add(ColorPoint(10/12, bcol(202,223,244)))
  bubbleBlueBg:add(ColorPoint(9/12, bcol(185,214,242)))
  bubbleBlueBg:add(ColorPoint(8/12, bcol(170,207,240)))
  bubbleBlueBg:add(ColorPoint(7/12, bcol(151,196,238)))
  bubbleBlueBg:add(ColorPoint(6/12, bcol(125,183,235)))
  bubbleBlueBg:add(ColorPoint(5/12, bcol(120,181,236)))
  bubbleBlueBg:add(ColorPoint(4/12, bcol(126,186,240)))
  bubbleBlueBg:add(ColorPoint(3/12, bcol(143,198,248)))
  bubbleBlueBg:add(ColorPoint(2/12, bcol(164,217,255)))
  bubbleBlueBg:add(ColorPoint(1/12, bcol(185,240,255)))
  bubbleBlueBg:add(ColorPoint(0/12, bcol(195,252,255)))
  textureManager:addGradient("bubbleBlueBg", bubbleBlueBg)  
end

local function candyGradients(textureManager)
  local segGreyFrame = ColorGradient.create()
  segGreyFrame:add(ColorPoint(1, bgrey(120)))
  segGreyFrame:add(ColorPoint(0, bgrey(97)))
  textureManager:addGradient("candyGrayFrame", segGreyFrame)  
  
  local segBlueFrame = ColorGradient.create()
  segBlueFrame:add(ColorPoint(1, bcol(47,24,149)))
  segBlueFrame:add(ColorPoint(0.5, bcol(20,67,178)))
  segBlueFrame:add(ColorPoint(0, bgrey(96)))
  textureManager:addGradient("candyBlueFrame", segBlueFrame)  

  local segLightGrey = ColorGradient.create()
  segLightGrey:add(ColorPoint(18/18, bgrey(250)))
  segLightGrey:add(ColorPoint(17/18, bgrey(251)))
  segLightGrey:add(ColorPoint(16/18, bgrey(248)))
  segLightGrey:add(ColorPoint(15/18, bgrey(246)))
  segLightGrey:add(ColorPoint(14/18, bgrey(244)))
  segLightGrey:add(ColorPoint(13/18, bgrey(242)))
  segLightGrey:add(ColorPoint(12/18, bgrey(241)))
  segLightGrey:add(ColorPoint(11/18, bgrey(239)))
  segLightGrey:add(ColorPoint(10/18, bgrey(233)))
  segLightGrey:add(ColorPoint(9/18, bgrey(228)))
  segLightGrey:add(ColorPoint(8/18, bgrey(231)))
  segLightGrey:add(ColorPoint(7/18, bgrey(233)))
  segLightGrey:add(ColorPoint(6/18, bgrey(237)))
  segLightGrey:add(ColorPoint(5/18, bgrey(239)))
  segLightGrey:add(ColorPoint(4/18, bgrey(244)))
  segLightGrey:add(ColorPoint(3/18, bgrey(249)))
  segLightGrey:add(ColorPoint(2/18, bgrey(254)))
  segLightGrey:add(ColorPoint(1/18, bgrey(255)))
  segLightGrey:add(ColorPoint(0/18, bgrey(255)))
  textureManager:addGradient("candyGray", segLightGrey)  

  local segBlue = ColorGradient.create()
  segBlue:add(ColorPoint(17/17, bcol(239,244,253)))
  segBlue:add(ColorPoint(16/17, bcol(207,228,248)))
  segBlue:add(ColorPoint(15/17, bcol(196,223,247)))
  segBlue:add(ColorPoint(14/17, bcol(189,218,247)))
  segBlue:add(ColorPoint(13/17, bcol(182,216,246)))
  segBlue:add(ColorPoint(12/17, bcol(178,213,246)))
  segBlue:add(ColorPoint(11/17, bcol(172,210,245)))
  segBlue:add(ColorPoint(10/17, bcol(147,198,243)))
  segBlue:add(ColorPoint(9/17, bcol(114,181,240)))
  segBlue:add(ColorPoint(8/17, bcol(122,186,243)))
  segBlue:add(ColorPoint(7/17, bcol(131,192,246)))
  segBlue:add(ColorPoint(6/17, bcol(141,199,250)))
  segBlue:add(ColorPoint(5/17, bcol(151,206,255)))
  segBlue:add(ColorPoint(4/17, bcol(163,216,255)))
  segBlue:add(ColorPoint(3/17, bcol(176,230,255)))
  segBlue:add(ColorPoint(2/17, bcol(193,248,255)))
  segBlue:add(ColorPoint(1/17, bcol(202,255,255)))
  segBlue:add(ColorPoint(0/17, bcol(191,244,255)))
  textureManager:addGradient("candyBlue", segBlue)  
end

local function squareGlassGradient(textureManager)
  local cg = ColorGradient.create()
  cg:add(ColorPoint(17/17, bgrey(249)))
  cg:add(ColorPoint(16/17, bgrey(250)))
  cg:add(ColorPoint(15/17, bgrey(250)))
  cg:add(ColorPoint(14/17, bgrey(250)))
  cg:add(ColorPoint(13/17, bgrey(251)))
  cg:add(ColorPoint(12/17, bgrey(251)))
  cg:add(ColorPoint(11/17, bgrey(251)))
  cg:add(ColorPoint(10/17, bgrey(251)))
  cg:add(ColorPoint(9/17, bgrey(251)))
  cg:add(ColorPoint(8/17, bgrey(241)))
  cg:add(ColorPoint(7/17, bgrey(233)))
  cg:add(ColorPoint(6/17, bgrey(234)))
  cg:add(ColorPoint(5/17, bgrey(234)))
  cg:add(ColorPoint(4/17, bgrey(234)))
  cg:add(ColorPoint(3/17, bgrey(235)))
  cg:add(ColorPoint(2/17, bgrey(235)))
  cg:add(ColorPoint(1/17, bgrey(236)))
  cg:add(ColorPoint(0/17, bgrey(236)))
  textureManager:addGradient("squareGlass", cg)  
end

function pebbleBuildGradients(textureManager)
  bubbleGradients(textureManager)
  roundRectGradients(textureManager)
  candyGradients(textureManager)
  miscGradients(textureManager)
  squareGlassGradient(textureManager)
end
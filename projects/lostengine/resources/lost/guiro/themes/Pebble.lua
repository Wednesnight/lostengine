module("lost.guiro.themes", package.seeall) 

require("lost.common.Class")
require("lost.guiro.Theme")

require("lost.guiro.layer.Text")
require("lost.guiro.layer.RoundedRect")

lost.common.Class "lost.guiro.themes.Pebble" "lost.guiro.Theme" {}

local Color = lost.common.Color

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
    log.debug("WITH ALPHA! "..tostring(c4/255))
    result = Color(c1/255, c2/255, c3/255, c4/255)
  else
    result = Color(c1/255, c2/255, c3/255, 1)
  end
  return result
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

local function segmentedGradients(textureManager)
  local segGreyFrame = ColorGradient.create()
  segGreyFrame:add(ColorPoint(1, bgrey(120)))
  segGreyFrame:add(ColorPoint(0, bgrey(97)))
  textureManager:addGradient("segGreyFrame", segGreyFrame)  
  
  local segBlueFrame = ColorGradient.create()
  segBlueFrame:add(ColorPoint(1, bcol(47,24,149)))
  segBlueFrame:add(ColorPoint(0.5, bcol(20,67,178)))
  segBlueFrame:add(ColorPoint(0, bgrey(96)))
  textureManager:addGradient("segBlueFrame", segBlueFrame)  

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
  textureManager:addGradient("segLightGrey", segLightGrey)  

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
  textureManager:addGradient("segBlue", segBlue)  
end

local function buildGradients(textureManager)
  bubbleGradients(textureManager)
  roundRectGradients(textureManager)
  segmentedGradients(textureManager)
end

function Pebble:constructor()
  lost.guiro.Theme.constructor(self)
  buildGradients(lost.guiro.textureManager())
  self.name = "pebble"
  self:addStyle("lost.guiro.view.Label", "default", function(target, args) self:labelDefault(target, args) end)
  self:addStyle("lost.guiro.view.Label", "round", function(target, args) self:labelRound(target, args) end)
  self:addStyle("lost.guiro.view.Label", "roundFramed", function(target, args) self:labelRoundFramed(target, args) end)
  self:addStyle("lost.guiro.view.Button", "default", function(target, args) self:buttonGray(target, args) end)
  self:addStyle("lost.guiro.view.Button", "rounded", function(target, args) self:buttonRounded(target, args) end)
  self:addStyle("lost.guiro.view.View", "gray", function(target, args) self:viewGray(target, args) end)
  self:addStyle("lost.guiro.view.UserInterface", "default", function(target, args) self:viewGray(target, args) end)
  
  self.buttonRoundedHeight = {mini=14, small=16, regular=18}
  self.buttonRoundedFonts = {mini={"Vera", 9}, small={"Vera", 10}, regular={"Vera", 11}}
  self.buttonRoundedFrameCol = Color(.6588,.6588,.6588)
--  self:addStyle("lost.guiro.view.View", "default", function(target, args) self:viewGray(target, args) end)
end

function Pebble:labelDefault(target, args)
  target.layer:addSublayer(lost.guiro.layer.Text{bounds={0,0,"1","1"},color=Color(0,0,0),font = {"Vera", 12}})
end

function Pebble:labelRound(target, args)
  target.layer:addSublayer(lost.guiro.layer.RoundedRect{bounds={0,0,"1","1"},color=Color(1,1,1),radius=8,filled=true})
  target.layer:addSublayer(lost.guiro.layer.Text{bounds={0,0,"1","1"},color = Color(0,0,0),font = {"Vera", 12}})
end

function Pebble:labelRoundFramed(target, args)
  target.layer:addSublayer(lost.guiro.layer.RoundedRect{bounds={0,0,"1","1"},color=Color(1,1,1),radius=8,filled=true})
  target.layer:addSublayer(lost.guiro.layer.RoundedRect{bounds={0,0,"1","1"},color=Color(0,0,0),radius=8,filled = false,width=1})
  target.layer:addSublayer(lost.guiro.layer.Text{bounds={0,0,"1","1"},color=Color(0,0,0),font={"Vera", 12}})
end

function Pebble:buttonGray(target,args)
  local b = lost.guiro.view.Button
  local normal = lost.guiro.layer.RoundedRect{bounds = {0,0,"1","1"},color=Color(1,0,0),filled=true,radius=8}
  local pushed = lost.guiro.layer.RoundedRect{bounds = {0,0,"1","1"},color = Color(1,1,0),filled = true,radius = 8}
  local disabled = lost.guiro.layer.RoundedRect{bounds={0,0,"1","1"},color=Color(.3,.3,.3),filled=true,radius=8}
  target.layer:addSublayer(normal)
  target.layer:addSublayer(pushed)
  target.layer:addSublayer(disabled)
  target.backgrounds[b.STATE_NORMAL] = normal
  target.backgrounds[b.STATE_PUSHED] = pushed
  target.backgrounds[b.STATE_DISABLED] = disabled
end

function Pebble:buttonRounded(target,args)
  local l = lost.guiro.layer.Layer
  local rr = lost.guiro.layer.RoundedRect

  local b = lost.guiro.view.Button
  local size = args.size or "small"
  -- target bounds must have been set by now so we can modify height
  target._bounds.height = lost.guiro.Bounds.decodeEntry(4,self.buttonRoundedHeight[size])
  local r = self.buttonRoundedHeight[size]/2 -- rounded rect radius
  local normal = l{sublayers={rr{bounds={0,0,"1","1"},gradient="rrbg",filled=true,radius=r},
                              rr{bounds={0,0,"1","1"},color=self.buttonRoundedFrameCol,filled=false,radius=r}}
                             }
  local pushed = l{sublayers={rr{bounds={0,0,"1","1"},gradient="rrbg2",filled=true,radius=r},
                              rr{bounds={0,0,"1","1"},color=self.buttonRoundedFrameCol,filled=false,radius=r}}
                             }
  local text = lost.guiro.layer.Text{bounds = {0,0,"1","1"},font = self.buttonRoundedFonts[size], color=Color(0,0,0),text="Rounded Button"}
  target.layer:addSublayer(normal)
  target.layer:addSublayer(pushed)
  target.layer:addSublayer(text)
  target.textLayer = text
  target.backgrounds[b.STATE_NORMAL] = normal
  target.backgrounds[b.STATE_PUSHED] = pushed
end


function Pebble:viewGray(target, args)
  target.layer:addSublayer(lost.guiro.layer.Rect{bounds={0,0,"1","1"},color = Color(.9294,.9294,.9294),filled = true})  
end

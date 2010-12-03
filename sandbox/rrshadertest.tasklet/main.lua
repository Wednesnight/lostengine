require("lost.declarative.Context")
require("lost.common.Shaders")
require("lost.common.ShaderFactory")
require("lost.common.MeshFactory")
require("lost.guiro.layer.Rect")
require("lost.guiro.layer.RoundedRect")
require("lost.guiro.layer.Text")

using "lost.math.Vec2"
using "lost.math.Vec3"
using "lost.math.Rect"
using "lost.common.Color"
using "lost.bitmap.Bitmap"
using "lost.math.MatrixTranslation"
using "lost.math.MatrixRotZ"

config = require("config")
running = true
dcl = nil
sum = 0
curwidth = 3
shaderFactory = lost.common.ShaderFactory(tasklet.loader)
meshFactory = lost.common.MeshFactory(shaderFactory)

white = Color(1,1,1,1)
yellow = Color(1,1,0,1)
red = Color(1,0,0,1)
blue = Color(0,0,1,1)
gray1 = Color(.2, .2, .2, 1)
gray2 = Color(.8, .8, .8, 1)
green = Color(0, 1, 0, 1)

r = 8
w = 3

left = 20
right = 320


local ColorGradient = lost.common.ColorGradient
local ColorPoint = lost.common.ColorPoint
local Color = lost.common.Color

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
    log.debug("WITH ALPHA! "..tostring(c4/255))
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

function segmentedGradients(textureManager)
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

function buildGradients(textureManager)
  bubbleGradients(textureManager)
  roundRectGradients(textureManager)
  segmentedGradients(textureManager)
end

function layerTest()
  dcl.guiro:UserInterface
  {
    id = "ui"
  }

  local Layer = lost.guiro.layer.Layer
  local lrect = lost.guiro.layer.Rect
  local rrect = lost.guiro.layer.RoundedRect
  local text = lost.guiro.layer.Text

  buildGradients(ui.textureManager)


  rootLayer = Layer
              {
                id="rootLayer", 
                bounds={0,0,config.window.width,config.window.height},
                sublayers = 
                {
                  lrect{id="sl1", bounds={"left","top",50,50}, color=Color(1,1,1), gradient="segBlueFrame"},
                  lrect
                  {
                      id="sl2",bounds={50,0,50,50},color=Color(.6,.6,.6),
                      gradient="segLightGrey",
                      filled = false,
                      width=12,
                      sublayers=
                      {
                        lrect{id="sl3", bounds={"right", "top", ".5", ".5"}, color=Color(0,0,1)}
                      }
                  },
                  lrect{
                          id="sl4", bounds={10,10,20,20}, color=Color(1,1,1),
                          filled = true,
                          width = 3,
                          gradient="segBlue"
                          },
                  rrect
                  {
                    id="rr1",
                    bounds = {".3", {".5", -10}, 200, 200},
                    color = Color(1,0,0),
                    sides = {top=true, bottom=false, left=true, right=true},
                    roundCorners = {tl=false, tr=true, bl=true, br=true},
                    radius = 24,
                    width = 12,
                    filled = false
                  },
                  rrect
                  {
                    id="rr2",
                    bounds = {{".3", 220}, {".5", -10}, 200, 200},
                    color = Color(1,1,1),
                    sides = {top=true, bottom=true, left=true, right=true},
                    roundCorners = {tl=false, tr=true, bl=true, br=true},
                    radius = 24,
                    width = 12,
                    filled = false,
                    gradient = "segBlue"
                  },
                  Layer
                  {
                    id="textContainer",
                    bounds={400,100,350,50},
                    sublayers=
                    {
                      --lrect{bounds={0,0,"1","1"}, color=Color(.8,.8,.8), },
                      rrect{bounds={0,0,"1","1"},radius=8,width=4,filled=true, gradient="segBlue"},
                      text
                      {
                        id="text",
                        bounds = {0,0,"1","1"},
                        color=Color(0,0,0),
                        font = {"Vera", 17},
                        text = "Hello new text layer! | Mini",
                        valign="center",
                        halign = "center"
                      },
                      rrect{bounds={0,0,"1","1"},radius=8,width=1,filled=false,gradient="segBlueFrame"}
                    },
                  }
                }
              }
  rootLayer:print()
  rootLayer.renderNode:print()

  scene2:add(rootLayer.renderNode)

--[[  sl1:removeFromSuperlayer()
  rootLayer:print()
  rootLayer.renderNode:print()

  sl3:removeFromSuperlayer()
  rootLayer:print()
  rootLayer.renderNode:print()

  sl2:removeFromSuperlayer()
  rootLayer:print()
  rootLayer.renderNode:print()

  sl4:removeFromSuperlayer()
  rootLayer:print()
  rootLayer.renderNode:print()]]
end

function startup()
  dcl = lost.declarative.Context(tasklet.loader)
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)
  
  scene1 = require("scene1")
  scene2 = require("scene2")
  tasklet.renderNode:add(scene1)
  tasklet.renderNode:add(scene2)
  scene1.active = false

  layerTest()

  return running
end

function update(dt)  
  return running
end

function keyHandler(event)
  if event.key == lost.application.K_ESCAPE then
    running = false
  elseif event.key == lost.application.K_1 then
    scene1.active = true
    scene2.active = false
  elseif event.key == lost.application.K_2 then
    scene1.active = false
    scene2.active = true
  end
end


require("lost.declarative.Context")
require("lost.common.Shaders")
require("lost.common.ShaderFactory")
require("lost.common.MeshFactory")

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

function layerTest()
  dcl.guiro:UserInterface
  {
    id = "ui"
  }

  rootLayer = lost.guiro.layer.Layer{id="rootLayer", bounds={0,0,200,200}}
  rootLayer:needsLayout()
  sl1 = lost.guiro.layer.Layer{id="sl1", bounds={0,0,50,50}}
  sl2 = lost.guiro.layer.Layer{id="sl2", bounds={50,0,50,50}}
  sl3 = lost.guiro.layer.Layer{id="sl3", bounds={"top", "left", ".5", ".5"}}
  sl4 = lost.guiro.layer.Layer{id="sl4", bounds={10,10,20,20}}
  rootLayer:addSublayer(sl1)
  rootLayer:addSublayer(sl2)
  sl2:addSublayer(sl3)

  rootLayer:addSublayer(sl4)
  rootLayer:print()
  rootLayer._renderNode:print()

--[[  sl1:removeFromSuperlayer()
  rootLayer:print()
  rootLayer._renderNode:print()

  sl3:removeFromSuperlayer()
  rootLayer:print()
  rootLayer._renderNode:print()

  sl2:removeFromSuperlayer()
  rootLayer:print()
  rootLayer._renderNode:print()

  sl4:removeFromSuperlayer()
  rootLayer:print()
  rootLayer._renderNode:print()]]
end

function startup()
  dcl = lost.declarative.Context(tasklet.loader)
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)
  
  scene1 = require("scene1")
  scene2 = require("scene2")
  tasklet.renderNode:add(scene1)
  tasklet.renderNode:add(scene2)
  scene2.active = false

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


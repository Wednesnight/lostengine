require("lost.declarative.Context")
require("lost.common.Shaders")
require("lost.common.ShaderFactory")
require("lost.common.MeshFactory")
require("lost.guiro.layer.Rect")
require("lost.guiro.layer.RoundedRect")

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

  local Layer = lost.guiro.layer.Layer
  local lrect = lost.guiro.layer.Rect
  local rrect = lost.guiro.layer.RoundedRect

  rootLayer = Layer
              {
                id="rootLayer", 
                bounds={0,0,config.window.width,config.window.height},
                sublayers = 
                {
                  lrect{id="sl1", bounds={"left","top",50,50}, color=Color(1,0,0)},
                  lrect
                  {
                      id="sl2",bounds={50,0,50,50},color=Color(0,1,0),
                      sublayers=
                      {
                        lrect{id="sl3", bounds={"right", "top", ".5", ".5"}, color=Color(0,0,1)}
                      }
                  },
                  lrect{
                          id="sl4", bounds={10,10,20,20}, color=Color(1,1,0),
                          filled = false,
                          width = 3
                          },
                  rrect
                  {
                    id="rr1",
                    bounds = {".5", ".5", 200, 200},
                    color = Color(1,0,1)
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


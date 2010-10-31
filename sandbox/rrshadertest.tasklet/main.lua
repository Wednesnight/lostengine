require("lost.declarative.Context")
require("lost.common.Shaders")
require("ShaderFactory")
require("MeshFactory")

using "lost.math.Vec2"
using "lost.math.Vec3"
using "lost.math.Rect"
using "lost.common.Color"
using "lost.bitmap.Bitmap"
using "lost.math.MatrixTranslation"
using "lost.math.MatrixRotZ"

config = require("config")
running = true
rootNode = nil
dcl = nil
sum = 0
curwidth = 3
shaderFactory = lost.common.ShaderFactory(tasklet.loader)
meshFactory = lost.common.MeshFactory(shaderFactory)

function createComboRectNode(col1, col2, rect, radius, width)
  local result = dcl.rg:Node
  {
    dcl.rg:Draw{mesh = meshFactory:createRoundedRect(col1, rect, radius)},
    dcl.rg:Draw{mesh = meshFactory:createRoundedRectFrame(col2, rect, radius, width)},
  }
  return result
end

function iqcreateComboRectNode(col1, col2, rect, radius, width)
  local result = dcl.rg:Node
  {
    dcl.rg:Draw{mesh = meshFactory:iqcreateRoundedRect(col1, rect, radius)},
    dcl.rg:Draw{mesh = meshFactory:iqcreateRoundedRectFrame(col2, rect, radius, width)},
  }
  return result
end

--function roundedRect(col, rect, radius, )

function startup()
  dcl = lost.declarative.Context(tasklet.loader)
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)
  
  local white = Color(1,1,1,1)
  local red = Color(1,0,0,1)
  local blue = Color(0,0,1,1)
  local gray1 = Color(.2, .2, .2, 1)
  local gray2 = Color(.8, .8, .8, 1)
  local green = Color(0, 1, 0, 1)

  local r = 4
  local w = 2

  local left = 20
  local right = 320

  rootNode = dcl.rg:Node
  {
    dcl.rg:ClearColor { color = Color(92/255,174/255,210/255,1) },
    dcl.rg:Clear { mask = gl.GL_COLOR_BUFFER_BIT },
    dcl.rg:DepthTest{false},
    dcl.rg:Camera2D { viewport = Rect(0,0,config.window.width, config.window.height) },    

    createComboRectNode(gray2, gray1, Rect(left,390,200,200), 27, 2),
    iqcreateComboRectNode(gray2, gray1, Rect(right,390,200,200), 27, 2),
    
    dcl.rg:Draw { mesh = meshFactory:createRoundedRectFrame(white, Rect(20,180, 10, 10), r,w) },
    dcl.rg:Draw { mesh = meshFactory:createRoundedRectFrame(white, Rect(20,20, 16, 16), r,w) },
    dcl.rg:Draw { mesh = meshFactory:createRoundedRectFrame(white, Rect(100,50, 64, 64), r,w) },
    dcl.rg:Draw { mesh = meshFactory:createRoundedRectFrame(white, Rect(10,100, 13, 13), r,w) },
    dcl.rg:Draw { mesh = meshFactory:createRoundedRectFrame(white, Rect(200,200, 57, 57), r,w) },
    dcl.rg:Draw { mesh = meshFactory:createRoundedRectFrame(white, Rect(420,200, 64, 46), r,w) },

    createComboRectNode(gray2, gray1, Rect(left, 350, 120, 20), r, 1),
    iqcreateComboRectNode(gray2, gray1, Rect(right, 350, 120, 20), r, 1),
    
    dcl.rg:Draw { mesh = meshFactory:createRoundedRectFrame(white, Rect(left,230, 100, 100), r,w) },
    dcl.rg:Draw { mesh = meshFactory:iqcreateRoundedRectFrame(white, Rect(right,230, 100, 100), r,w) },

    dcl.rg:Draw { mesh = meshFactory:createRoundedRectFrame(white, Rect(left,30, 100, 100), r,1) },
    dcl.rg:Draw { mesh = meshFactory:iqcreateRoundedRectFrame(white, Rect(right, 30, 100, 100), r,1) },

    dcl.rg:Draw { mesh = meshFactory:createDisc(white, Vec2(600,300), 47) },

    dcl.rg:Draw { mesh = meshFactory:iqcreateRoundedRectFrame(white, Rect(right,180, 10, 10), r,w) },

    dcl.rg:Draw { mesh = meshFactory:createBox(white, Rect(right,10, 400, 400)) },
  }    
  tasklet.renderNode:add(rootNode)

  return running
end

function update(dt)  
  return running
end

function keyHandler(event)
  if (event.key == lost.application.K_ESCAPE) then
    running = false
  end
end


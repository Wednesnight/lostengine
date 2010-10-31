require("lost.declarative.Context")
require("lost.common.Shaders")
require("ShaderFactory")

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

function createQuad(size)
  local layout = lost.gl.BufferLayout()
  layout:add(gl.ET_vec2_f32, gl.UT_position, 0)
  layout:add(gl.ET_vec2_f32, gl.UT_texcoord0, 0)
  local result = lost.mesh.Mesh.create(layout, gl.ET_u16)
  result.indexBuffer.drawMode = gl.GL_TRIANGLES
  local numVerts = 4
  local numTris = 2
  local numIndicesPerTri = 3
  local numIndices = numTris * numIndicesPerTri
  result:resetSize(numVerts, numIndices)

  result:set(0,gl.UT_position, Vec2(0,0))
  result:set(1,gl.UT_position, Vec2(size.x,0))
  result:set(2,gl.UT_position, Vec2(size.x,size.y))
  result:set(3,gl.UT_position, Vec2(0,size.y))

  result:set(0,gl.UT_texcoord0, Vec2(0,0))
  result:set(1,gl.UT_texcoord0, Vec2(1,0))
  result:set(2,gl.UT_texcoord0, Vec2(1,1))
  result:set(3,gl.UT_texcoord0, Vec2(0,1))

  result:setU16(0,gl.UT_index, 0)
  result:setU16(1,gl.UT_index, 1)
  result:setU16(2,gl.UT_index, 2)
  result:setU16(3,gl.UT_index, 2)
  result:setU16(4,gl.UT_index, 3)
  result:setU16(5,gl.UT_index, 0)
  
  result.material.uniforms = lost.gl.UniformBlock.create()
  result.material:blendPremultiplied()
  
  return result
end

function createDisc(col, pos, radius)
  local size = Vec2(radius*2, radius*2)
  local result = createQuad(size)
  result.material.color = col
  result.material.shader = shaderFactory:disc()
  result.material.uniforms:set("size", size)
  result.material.uniforms:set("center", Vec2(radius-1, radius-1))
  result.material.uniforms:setFloat("radius", radius)
  result.transform = MatrixTranslation(Vec3(pos.x,pos.y,0))  
  return result
end

function createRing(col, pos, radius, width)
  local size = Vec2(radius*2, radius*2)
  local result = createQuad(size)
  result.material.color = col
  result.material.shader = shaderFactory:ring()
  result.material.uniforms:set("size", size)
  result.material.uniforms:set("center", Vec2(radius-1, radius-1))
  result.material.uniforms:setFloat("radius", radius)
  result.material.uniforms:setFloat("width", width)
  result.transform = MatrixTranslation(Vec3(pos.x,pos.y,0))  
  return result
end

function createRoundedRect(col, rect, radius)
  local size = Vec2(rect.width, rect.height)
  local result = createQuad(size)

  result.material.color = col
  result.material.shader = shaderFactory:roundedRect(true, {}, {}) --buildRRShader(true,{tl=true, tr=true, bl=true, br=true}, {}) --loadShader("roundedRect")
  result.material.uniforms:set("size", size)
  result.material.uniforms:setFloat("radius", radius)
  result.transform = MatrixTranslation(Vec3(rect.x,rect.y,0))
--  result.material:blendOff();
  
  return result
end

function createRoundedRectFrame(col, rect, radius, width)
  local size = Vec2(rect.width, rect.height)
  local result = createQuad(size)

  result.material.color = col
  result.material.shader = shaderFactory:roundedRect(false, {}, {}) --loadShader("roundedRectFrame")
  result.material.uniforms:set("size", size)
  result.material.uniforms:setFloat("radius", radius)
  result.material.uniforms:setFloat("width", width)
  result.transform = MatrixTranslation(Vec3(rect.x,rect.y,0))
  
  return result
end

function createBox(col, rect)
  local size = Vec2(rect.width, rect.height)
  local result = createQuad(size)
  result.material.color = col
  result.material.shader = shaderFactory:box()
  result.material.uniforms:set("size", size)
  result.transform = MatrixTranslation(Vec3(rect.x,rect.y,0))*MatrixRotZ(-87)
  result.material:blendPremultiplied()
  return result
end

function iqcreateRoundedRect(col, rect, radius)
  local result = createRoundedRect(col, rect, radius)
  result.material.shader = shaderFactory:iqrr()
  return result
end

function iqcreateRoundedRectFrame(col, rect, radius, width)
  local result = createRoundedRectFrame(col, rect, radius, width)
  result.material.shader = shaderFactory:iqrrf()
  return result
end

function createComboRectNode(col1, col2, rect, radius, width)
  local result = dcl.rg:Node
  {
    dcl.rg:Draw{mesh = createRoundedRect(col1, rect, radius)},
    dcl.rg:Draw{mesh = createRoundedRectFrame(col2, rect, radius, width)},
  }
  return result
end

function iqcreateComboRectNode(col1, col2, rect, radius, width)
  local result = dcl.rg:Node
  {
    dcl.rg:Draw{mesh = iqcreateRoundedRect(col1, rect, radius)},
    dcl.rg:Draw{mesh = iqcreateRoundedRectFrame(col2, rect, radius, width)},
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
--    dcl.rg:Draw { mesh = createDisc(white, Vec2(600,300), 47) },
--    dcl.rg:Draw { mesh = createRing(red, Vec2(600,300), 47, 2) },
    
    dcl.rg:Draw { mesh = createRoundedRectFrame(white, Rect(20,180, 10, 10), r,w) },
    dcl.rg:Draw { mesh = createRoundedRectFrame(white, Rect(20,20, 16, 16), r,w) },
    dcl.rg:Draw { mesh = createRoundedRectFrame(white, Rect(100,50, 64, 64), r,w) },
    dcl.rg:Draw { mesh = createRoundedRectFrame(white, Rect(10,100, 13, 13), r,w) },
    dcl.rg:Draw { mesh = createRoundedRectFrame(white, Rect(200,200, 57, 57), r,w) },
    dcl.rg:Draw { mesh = createRoundedRectFrame(white, Rect(420,200, 64, 46), r,w) },

    createComboRectNode(gray2, gray1, Rect(left, 350, 120, 20), r, 1),
    iqcreateComboRectNode(gray2, gray1, Rect(right, 350, 120, 20), r, 1),
    
    dcl.rg:Draw { mesh = createRoundedRectFrame(white, Rect(left,230, 100, 100), r,w) },
    dcl.rg:Draw { mesh = iqcreateRoundedRectFrame(white, Rect(right,230, 100, 100), r,w) },

--    dcl.rg:Draw { mesh = createRoundedRect(white, Rect(left,150, 80, 50), r) },
--    dcl.rg:Draw { mesh = iqcreateRoundedRect(white, Rect(right,150, 80, 50), r) },

    dcl.rg:Draw { mesh = createRoundedRectFrame(white, Rect(left,30, 100, 100), r,1) },
    dcl.rg:Draw { mesh = iqcreateRoundedRectFrame(white, Rect(right, 30, 100, 100), r,1) },

    dcl.rg:Draw { mesh = createDisc(white, Vec2(600,300), 47) },

    dcl.rg:Draw { mesh = iqcreateRoundedRectFrame(white, Rect(right,180, 10, 10), r,w) },

    dcl.rg:Draw { mesh = createBox(white, Rect(right,10, 400, 400)) },
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


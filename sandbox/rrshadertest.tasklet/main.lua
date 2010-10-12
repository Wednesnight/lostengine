require("lost.declarative.Context")
require("lost.common.Shaders")

using "lost.math.Vec2"
using "lost.math.Vec3"
using "lost.math.Rect"
using "lost.common.Color"
using "lost.bitmap.Bitmap"
using "lost.math.MatrixTranslation"

config = require("config")
running = true
rootNode = nil
dcl = nil

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
  
  return result
end

function createRR(pos, radius)
  local size = Vec2(radius*2, radius*2)
  local result = createQuad(size)

  local shaderParams = lost.gl.UniformBlock.create()
  shaderParams:set("size", size)
  shaderParams:set("center", Vec2(radius-1,radius-1))
  shaderParams:setFloat("radius", radius)

  result.material.color = Color(1,1,1,1)
  result.material.shader = lost.gl.loadShader(tasklet.loader, "rr")
  result.material.uniforms = shaderParams
  result.material:blendPremultiplied()
  result.transform = MatrixTranslation(Vec3(pos.x,pos.y,0))
  
  return result
end

function startup()
  dcl = lost.declarative.Context(tasklet.loader)
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)

  rootNode = dcl.rg:Node
  {
    dcl.rg:ClearColor { color = Color(92/255,174/255,210/255,1) },
    dcl.rg:Clear { mask = gl.GL_COLOR_BUFFER_BIT },
    dcl.rg:DepthTest{false},
    dcl.rg:Camera2D { viewport = Rect(0,0,config.window.width, config.window.height) },    
    dcl.rg:Draw { mesh = createRR(Vec2(20,20), 16) },
    dcl.rg:Draw { mesh = createRR(Vec2(100,50), 64) },
    dcl.rg:Draw { mesh = createRR(Vec2(10,100), 13) },
    dcl.rg:Draw { mesh = createRR(Vec2(200,200), 57) },
    dcl.rg:Draw { mesh = createRR(Vec2(420,300), 164) },
    dcl.rg:Draw { mesh = createRR(Vec2(400,187), 47) },
  }    
  tasklet.renderNode:add(rootNode)

  return running
end

function update()
  return running
end

function keyHandler(event)
  if (event.key == lost.application.K_ESCAPE) then
    running = false
  end
end


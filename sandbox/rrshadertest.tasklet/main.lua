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

function startup()
  dcl = lost.declarative.Context(tasklet.loader)
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)

  local shaderParams = lost.gl.UniformBlock.create()
  local quadSize = Vec2(200,200)
  shaderParams:set("size", quadSize)
  shaderParams:set("center", Vec2(50,50))
  shaderParams:setFloat("radius", 20)

  local layout = lost.gl.BufferLayout()
  layout:add(gl.ET_vec2_f32, gl.UT_position, 0)
  layout:add(gl.ET_vec2_f32, gl.UT_texcoord0, 0)
  local rrmesh = lost.mesh.Mesh.create(layout, gl.ET_u16)
  rrmesh.indexBuffer.drawMode = gl.GL_TRIANGLES
  local numVerts = 4
  local numTris = 2
  local numIndicesPerTri = 3
  local numIndices = numTris * numIndicesPerTri
  rrmesh:resetSize(numVerts, numIndices)
  
  rrmesh:set(0,gl.UT_position, Vec2(0,0))
  rrmesh:set(1,gl.UT_position, Vec2(quadSize.x,0))
  rrmesh:set(2,gl.UT_position, Vec2(quadSize.x,quadSize.y))
  rrmesh:set(3,gl.UT_position, Vec2(0,quadSize.y))

  rrmesh:set(0,gl.UT_texcoord0, Vec2(0,0))
  rrmesh:set(1,gl.UT_texcoord0, Vec2(1,0))
  rrmesh:set(2,gl.UT_texcoord0, Vec2(1,1))
  rrmesh:set(3,gl.UT_texcoord0, Vec2(0,1))

  
  rrmesh:setU16(0,gl.UT_index, 0)
  rrmesh:setU16(1,gl.UT_index, 1)
  rrmesh:setU16(2,gl.UT_index, 2)
  rrmesh:setU16(3,gl.UT_index, 2)
  rrmesh:setU16(4,gl.UT_index, 3)
  rrmesh:setU16(5,gl.UT_index, 0)
  
  rrmesh.material.color = Color(1,0,0)
  rrmesh.material.shader = lost.gl.loadShader(tasklet.loader, "rr")
  rrmesh.material.uniforms = shaderParams
  rrmesh.transform = MatrixTranslation(Vec3(20,20,0))

  rootNode = dcl.rg:Node
  {
    dcl.rg:ClearColor
    {
      color = Color(92/255,174/255,210/255,0)
    },
    dcl.rg:Clear
    {
      mask = gl.GL_COLOR_BUFFER_BIT
    },
    dcl.rg:DepthTest{false},
    dcl.rg:Camera2D
    {
      viewport = Rect(0,0,config.window.width, config.window.height)
    },    
    dcl.rg:Draw
    {
        mesh = rrmesh
    }
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


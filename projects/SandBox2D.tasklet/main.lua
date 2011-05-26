local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local MatrixTranslation = lost.math.MatrixTranslation
local MatrixRotZ = lost.math.MatrixRotZ
local Rect = lost.math.Rect
local Color = lost.common.Color

local PhysicsManager = require("PhysicsManager")
local World = require("World")
local Entity = require("Entity")

function createBoxMesh(size)
  local layout = lost.gl.BufferLayout()
  layout:add(gl.ET_vec2_f32, gl.UT_position, 0)
  layout:add(gl.ET_vec2_f32, gl.UT_texcoord0, 0)
  local mesh = lost.mesh.Mesh.create(layout, gl.ET_u16)
  mesh.indexBuffer.drawMode = gl.GL_TRIANGLES
  local numVerts = 4
  local numTris = 2
  local numIndicesPerTri = 3
  local numIndices = numTris * numIndicesPerTri
  mesh:resetSize(numVerts, numIndices)

  mesh:set(0,gl.UT_position, Vec2(0,0))
  mesh:set(1,gl.UT_position, Vec2(size.width,0))
  mesh:set(2,gl.UT_position, Vec2(size.width,size.height))
  mesh:set(3,gl.UT_position, Vec2(0,size.height))

  mesh:set(0,gl.UT_texcoord0, Vec2(0,0))
  mesh:set(1,gl.UT_texcoord0, Vec2(1,0))
  mesh:set(2,gl.UT_texcoord0, Vec2(1,1))
  mesh:set(3,gl.UT_texcoord0, Vec2(0,1))

  mesh:setU16(0,gl.UT_index, 0)
  mesh:setU16(1,gl.UT_index, 1)
  mesh:setU16(2,gl.UT_index, 2)
  mesh:setU16(3,gl.UT_index, 2)
  mesh:setU16(4,gl.UT_index, 3)
  mesh:setU16(5,gl.UT_index, 0)

  mesh.material.uniforms = lost.gl.UniformBlock.create()
  mesh.material:blendPremultiplied()
  return mesh
end

function addBoxNode(pos,sz,rotz)
--  local sz = Vec2(100,100)
  local boxMesh = createBoxMesh(sz)
   boxMesh.material.shader = lost.gl.loadShader(tasklet.loader,"box")
   boxMesh.material.color = Color(1,1,1)
   boxMesh.transform = MatrixTranslation(Vec3(pos.x,pos.y,0))*MatrixRotZ(rotz)
   boxMesh.material.uniforms:set("size",sz)
  local drawNode = lost.rg.Draw.create(boxMesh)
  tasklet.renderNode:add(drawNode)
end

function startup()
  world = World()
  pm = PhysicsManager()
  
  groundBodyDef = box2d.b2BodyDef()
  groundBodyDef.position:Set(0.0,-10.0)
  groundBody = pm.world:CreateBody(groundBodyDef)
  
  groundBox = box2d.b2PolygonShape()
  groundBox:SetAsBox(50.0,10.0)
  groundBody:CreateFixture(groundBox,0)
  
  bodyDef = box2d.b2BodyDef()
  bodyDef.type = box2d.b2_dynamicBody
  bodyDef.position:Set(0,4)
  body = pm.world:CreateBody(bodyDef)
  dynamicBox = box2d.b2PolygonShape()
  dynamicBox:SetAsBox(1,1)
  
  fixtureDef = box2d.b2FixtureDef()
  fixtureDef:setShape(dynamicBox)
--  fixtureDef.shape = dynamicBox
  fixtureDef.density = 1
  fixtureDef.friction = .3
  
  fixture = body:CreateFixture(fixtureDef)
    
  tasklet.clearNode.active = false
  
  clearNode = lost.rg.Node.create()
  clearNode:add(lost.rg.ClearColor.create(Color(2/255,73/255,152/255)))
  clearNode:add(lost.rg.Clear.create(gl.GL_COLOR_BUFFER_BIT))
  tasklet.renderNode:add(clearNode)
  
  cam = lost.camera.Camera2D.create(Rect(0,0,800,800))
  camNode = lost.rg.Camera.create(cam)
  tasklet.renderNode:add(camNode)
  
  addBoxNode(Vec2(50,50),Vec2(100,100),17)
  addBoxNode(Vec2(200,200),Vec2(70,180),97)
  addBoxNode(Vec2(200,400),Vec2(30,180),7)
  addBoxNode(Vec2(400,400),Vec2(35,180),-13)
  addBoxNode(Vec2(300,300),Vec2(15,180),0)
  addBoxNode(Vec2(300,280),Vec2(15,180),90)
  addBoxNode(Vec2(30,280),Vec2(2,230),67)
--  addBoxNode(Vec2(50,50),Vec2(100,100),17)
end

function update(dt)
  pm:update(dt)
end

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end
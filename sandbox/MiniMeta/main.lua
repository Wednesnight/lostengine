local Color = lost.common.Color
local Rect = lost.math.Rect
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local MatrixTranslation = lost.math.MatrixTranslation
local MatrixRotY = lost.math.MatrixRotY
local MatrixRotX = lost.math.MatrixRotX
local MatrixRotZ = lost.math.MatrixRotZ

require("Cube")

mbdown = false
roty = 0
rotx = 0

function createQuad(rect,meshZ,mbZ,col)
  local layout = lost.gl.BufferLayout()
  layout:add(gl.ET_vec3_f32, gl.UT_position, 0)
  layout:add(gl.ET_vec3_f32, gl.UT_vertexAttrib0, 0)
  local mesh = lost.mesh.Mesh.create(layout, gl.ET_u16)
  mesh.indexBuffer.drawMode = gl.GL_TRIANGLES
  local numVerts = 4
  local numTris = 2
  local numIndicesPerTri = 3
  local numIndices = numTris * numIndicesPerTri
  mesh:resetSize(numVerts, numIndices)
  
  mesh:set(0,gl.UT_position, Vec3(rect.x,rect.y,meshZ))
  mesh:set(1,gl.UT_position, Vec3(rect.x+rect.width,rect.y,meshZ))
  mesh:set(2,gl.UT_position, Vec3(rect.x+rect.width,rect.y+rect.height,meshZ))
  mesh:set(3,gl.UT_position, Vec3(rect.x,rect.y+rect.height,meshZ))

  mesh:set(0,gl.UT_vertexAttrib0, Vec3(0,0,mbZ))
  mesh:set(1,gl.UT_vertexAttrib0, Vec3(1,0,mbZ))
  mesh:set(2,gl.UT_vertexAttrib0, Vec3(1,1,mbZ))
  mesh:set(3,gl.UT_vertexAttrib0, Vec3(0,1,mbZ))

  mesh:setU16(0,gl.UT_index, 0)
  mesh:setU16(1,gl.UT_index, 1)
  mesh:setU16(2,gl.UT_index, 2)
  mesh:setU16(3,gl.UT_index, 2)
  mesh:setU16(4,gl.UT_index, 3)
  mesh:setU16(5,gl.UT_index, 0)

  mesh.material.uniforms = lost.gl.UniformBlock.create()
  mesh.material:blendPremultiplied()
  mesh.material.cull = false
  mesh.material.color = col
  mesh.material.shader = metaShader
  return mesh
end

function startup()
  tasklet.clearNode.active = false

  metaShader = lost.gl.loadShader(tasklet.loader,"meta");

  rootNode = lost.rg.Node.create()
  rootNode:add(lost.rg.ClearColor.create(Color(0,0,0,1)))
  rootNode:add(lost.rg.Clear.create(gl.GL_COLOR_BUFFER_BIT+gl.GL_DEPTH_BUFFER_BIT))
  cam = lost.camera.Camera3D.create(Rect(0,0,tasklet.window.size.width,tasklet.window.size.height))
  cam:depth(Vec2(0.001,1000))
  cam:position(Vec3(100,00,200))
  cam:target(Vec3(0,0,0))
  rootNode:add(lost.rg.Camera.create(cam))

  local cubeSize = 200

  cube = meta.Cube{color=Color(.1,.6,.0),size=200,numPlanes=30}

  rootNode:add(lost.rg.DepthTest.create(true))
  rootNode:add(cube.renderNode)


  tasklet.renderNode:add(rootNode)

  tasklet.eventDispatcher:addEventListener(lost.application.ResizeEvent.TASKLET_WINDOW_RESIZE,resize)
  tasklet.eventDispatcher:addEventListener(lost.application.MouseEvent.MOUSE_MOVE,mouseMove)
  tasklet.eventDispatcher:addEventListener(lost.application.MouseEvent.MOUSE_UP,mouseUp)
  tasklet.eventDispatcher:addEventListener(lost.application.MouseEvent.MOUSE_DOWN,mouseDown)
end

function update()
end

function mouseMove(event)
  if mbdown then
    roty = roty + (event.pos.x-mousex)
    rotx = rotx + (event.pos.y-mousey)
    mousex = event.pos.x
    mousey = event.pos.y
    cube:transform(MatrixRotY(roty)*MatrixRotX(rotx))
  end
end

function mouseUp(event)
  mbdown = false
end

function mouseDown(event)
  mbdown = true
  mousex = event.pos.x
  mousey = event.pos.y
end

function resize(event)
  cam:viewport(Rect(0,0,tasklet.window.size.width,tasklet.window.size.height))
end

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end
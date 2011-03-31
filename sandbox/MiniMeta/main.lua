local Color = lost.common.Color
local Rect = lost.math.Rect
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local MatrixTranslation = lost.math.MatrixTranslation
local MatrixRotY = lost.math.MatrixRotY
local MatrixRotZ = lost.math.MatrixRotZ

function createQuad(rect,meshZ,mbZ)
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
  return mesh
end

function createMesh(meshZ,mbZ,col)
  local result = createQuad(lost.math.Rect(0,0,200,200),meshZ,mbZ)
  result.material.color = col
  result.material.shader = metaShader
  return result
end

function startup()
  tasklet.clearNode.active = false

  metaShader = lost.gl.loadShader(tasklet.loader,"meta");

  rootNode = lost.rg.Node.create()
  rootNode:add(lost.rg.ClearColor.create(Color(0,0,0,1)))
  rootNode:add(lost.rg.Clear.create(gl.GL_COLOR_BUFFER_BIT+gl.GL_DEPTH_BUFFER_BIT))
  cam = lost.camera.Camera3D.create(Rect(0,0,tasklet.window.size.width,tasklet.window.size.height))
  cam:depth(Vec2(0.001,1000))
  cam:position(Vec3(150,150,75))
  cam:target(Vec3(50,50,0))
  rootNode:add(lost.rg.Camera.create(cam))

  rootNode:add(lost.rg.DepthTest.create(false))
  rootNode:add(lost.rg.Draw.create(createMesh(0,0,Color(.1,1,0))))
  rootNode:add(lost.rg.Draw.create(createMesh(10,.1,Color(.2,.9,0))))
  rootNode:add(lost.rg.Draw.create(createMesh(20,.2,Color(.3,.8,0))))
  rootNode:add(lost.rg.Draw.create(createMesh(30,.3,Color(.4,.7,0))))
  rootNode:add(lost.rg.Draw.create(createMesh(40,.4,Color(.5,.6,0))))

  tasklet.renderNode:add(rootNode)

  tasklet.eventDispatcher:addEventListener(lost.application.ResizeEvent.TASKLET_WINDOW_RESIZE,resize)
end

function update()
end

function resize(event)
  cam:viewport(Rect(0,0,tasklet.window.size.width,tasklet.window.size.height))
end

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end
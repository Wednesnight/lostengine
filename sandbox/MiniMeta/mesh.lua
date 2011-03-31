local Color = lost.common.Color
local Rect = lost.math.Rect
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local MatrixTranslation = lost.math.MatrixTranslation
local MatrixRotY = lost.math.MatrixRotY
local MatrixRotZ = lost.math.MatrixRotZ

function createQuad(rect,z)
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
  
  mesh:set(0,gl.UT_position, Vec3(rect.x,rect.y,z))
  mesh:set(1,gl.UT_position, Vec3(rect.x+rect.width,rect.y,z))
  mesh:set(2,gl.UT_position, Vec3(rect.x+rect.width,rect.y+rect.height,z))
  mesh:set(3,gl.UT_position, Vec3(rect.x,rect.y+rect.height,z))

  mesh:set(0,gl.UT_vertexAttrib0, Vec3(0,0,0))
  mesh:set(1,gl.UT_vertexAttrib0, Vec3(1,0,0))
  mesh:set(2,gl.UT_vertexAttrib0, Vec3(1,1,0))
  mesh:set(3,gl.UT_vertexAttrib0, Vec3(0,1,0))

  mesh:setU16(0,gl.UT_index, 0)
  mesh:setU16(1,gl.UT_index, 1)
  mesh:setU16(2,gl.UT_index, 2)
  mesh:setU16(3,gl.UT_index, 2)
  mesh:setU16(4,gl.UT_index, 3)
  mesh:setU16(5,gl.UT_index, 0)

  mesh.material.uniforms = lost.gl.UniformBlock.create()
  mesh.material:blendOff()
  mesh.material.cull = false
  return mesh
end

function createMesh()
  local result = createQuad(lost.math.Rect(0,0,100,100),-10)
  result.material.color = Color(0,1,0)
  result.material.shader = lost.gl.loadShader(tasklet.loader,"meta")
  return result
end



function cq()
--  local rect = Rect(0,0,50,50)
end
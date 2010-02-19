module("cube", package.seeall)

using "lost.math.Vec3"
using "lost.math.Vec2"
using "lost.common.Color"

function create(sideLength, numPlanes)
  local layout = lost.gl.BufferLayout()
  layout:add(gl.ET_vec3_f32, gl.UT_vertex, 0)
  layout:add(gl.ET_vec3_f32, gl.UT_texcoord0, 0)
  layout:add(gl.ET_vec3_f32, gl.UT_normal, 0)

  local m = lost.mesh.Mesh.create(layout, gl.ET_u32)
  m.drawMode = gl.GL_TRIANGLES
  local numQuads = numPlanes * 2 -- back and front facing
  local numVertsPerQuad = 4
  local numIndicesPerQuad = 6
  local numVerts = numVertsPerQuad*numQuads
  local numIndices = numIndicesPerQuad*numQuads
  m:resetSize(numVerts, numIndices)
  
  -- create ascending quads first, this is the "front" side
  local y = -math.floor(numPlanes/2)
  local planeIndex = 0
  local spacing = sideLength / (numPlanes-1)
  local vertexOffset = 0
  local indexOffset = 0
  for i=0,numPlanes-1,1 do
    createQuad(m, 
               sideLength,
               y*spacing, 
               vertexOffset+numVertsPerQuad*planeIndex,
               indexOffset+numIndicesPerQuad*planeIndex,
               .5,
               1)
    y = y+1
    planeIndex = planeIndex +1
  end

  -- create descending quads ... this time from y-1 to 0 for correct draw order when viewed from other side
  y = math.floor(numPlanes/2)
  planeIndex = 0
  vertexOffset = numVerts / 2
  indexOffset = numIndices / 2
  for i=0,numPlanes-1,1 do
    createQuad(m, 
               sideLength,
               y*spacing, 
               vertexOffset+numVertsPerQuad*planeIndex,
               indexOffset+numIndicesPerQuad*planeIndex,
               .5,
               -1) -- reverses normal and winding for correct viewing form other side
    y = y-1
    planeIndex = planeIndex +1
  end
  

  m.material.shader = dcl.gl:Shader
  {
    filename = "disc"
  }
  m.material.color = Color(1,.5,0)
  m.material.blend = true
  m.material.shader:enable()
  m.material.shader:set("lightPosition", Vec3(200,200,200))
--  m.material.shader:setFloat("radius", .1)
  m.material.shader:disable()
  m.material.cull = true
  
  return m
end

-- up should be either 1 or -1. If it's < 0, the winding rule will be reversed
function createQuad(m, sideLength, y, vertOffset, indexOffset, radius, up)
  -- xz plane means y = 0
  local hl = sideLength / 2 -- precompute half side length to center plane around 0,0,0
  m:set(vertOffset+0, gl.UT_vertex, Vec3(-hl,y,hl)) -- front left
  m:set(vertOffset+1, gl.UT_vertex, Vec3(hl,y,hl)) -- front right
  m:set(vertOffset+2, gl.UT_vertex, Vec3(hl,y,-hl)) -- back right
  m:set(vertOffset+3, gl.UT_vertex, Vec3(-hl,y,-hl)) -- back left

  m:set(vertOffset+0, gl.UT_texcoord0, Vec3(0,0, radius))
  m:set(vertOffset+1, gl.UT_texcoord0, Vec3(1,0, radius))
  m:set(vertOffset+2, gl.UT_texcoord0, Vec3(1,1, radius))
  m:set(vertOffset+3, gl.UT_texcoord0, Vec3(0,1, radius))

  m:set(vertOffset+0, gl.UT_normal, Vec3(0,up,0))
  m:set(vertOffset+1, gl.UT_normal, Vec3(0,up,0))
  m:set(vertOffset+2, gl.UT_normal, Vec3(0,up,0))
  m:set(vertOffset+3, gl.UT_normal, Vec3(0,up,0))

  if up > 0 then  
    m:setU32(indexOffset+0, gl.UT_index, vertOffset+0)
    m:setU32(indexOffset+1, gl.UT_index, vertOffset+1)
    m:setU32(indexOffset+2, gl.UT_index, vertOffset+2)
    m:setU32(indexOffset+3, gl.UT_index, vertOffset+2)
    m:setU32(indexOffset+4, gl.UT_index, vertOffset+3)
    m:setU32(indexOffset+5, gl.UT_index, vertOffset+0)  
  else
    m:setU32(indexOffset+0, gl.UT_index, vertOffset+0)
    m:setU32(indexOffset+1, gl.UT_index, vertOffset+3)
    m:setU32(indexOffset+2, gl.UT_index, vertOffset+2)
    m:setU32(indexOffset+3, gl.UT_index, vertOffset+2)
    m:setU32(indexOffset+4, gl.UT_index, vertOffset+1)
    m:setU32(indexOffset+5, gl.UT_index, vertOffset+0)  
  end
end

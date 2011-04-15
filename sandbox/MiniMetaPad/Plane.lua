module("meta",package.seeall)

require("lost.common.Class")

local Vec3 = lost.math.Vec3

lost.common.Class "meta.Plane" {}

-- args = table with constructor params
-- Parameters:
-- mode : "xy","xz" or "zy" to define the direction of the plane
-- color : the color of the plane mesh
-- rect : x/y will be used according to mode (e.g. for mode "zy" it will be z=x and y=y) and width and height as "x" and "y" sizes
-- meshOffset : this will be the third missing coordinate, the plane in which it is created (e.g. for mode "xy" this will be used as "z") 
-- metaOffset : this is the third coordinate in the metaball space
-- shader from global var "metaShader" is assigned to each Plane instance
function Plane:constructor(args)
  local layout = lost.gl.BufferLayout()
  layout:add(gl.ET_vec3_f32, gl.UT_position, 0)
  layout:add(gl.ET_vec3_f32, gl.UT_normal, 0)
  layout:add(gl.ET_vec3_f32, gl.UT_vertexAttrib0, 0)
  self.mesh = lost.mesh.Mesh.create(layout, gl.ET_u16)
  self.mesh.indexBuffer.drawMode = gl.GL_TRIANGLES
  local numVerts = 4
  local numTris = 2
  local numIndicesPerTri = 3
  local numIndices = numTris * numIndicesPerTri
  self.mesh:resetSize(numVerts, numIndices)

  if args.mode == "xy" then self:xy(args)
  elseif args.mode == "xz" then self:xz(args)
  elseif args.mode == "zy" then self:zy(args)
  end

  self.mesh:setU16(0,gl.UT_index, 0)
  self.mesh:setU16(1,gl.UT_index, 1)
  self.mesh:setU16(2,gl.UT_index, 2)
  self.mesh:setU16(3,gl.UT_index, 2)
  self.mesh:setU16(4,gl.UT_index, 3)
  self.mesh:setU16(5,gl.UT_index, 0)

  self.mesh.material.uniforms = lost.gl.UniformBlock.create()
  self.mesh.material:blendOff()
  self.mesh.material.cull = false
  self.mesh.material.color = args.color
  self.mesh.material.shader = metaShader
  
end

function Plane:xy(args)
  local rect = args.rect
  local meshZ = args.meshOffset
  local mbZ = args.metaOffset
  local offset = args.offset

  local p0 = Vec3(rect.x,rect.y,meshZ)+offset
  local p1 = Vec3(rect.x+rect.width,rect.y,meshZ)+offset
  local p2 = Vec3(rect.x+rect.width,rect.y+rect.height,meshZ)+offset
  local v0 = p1 - p0
  local v1 = p2-p0
  local n = lost.math.cross(v0,v1)
  lost.math.normalise(n)

  self.mesh:set(0,gl.UT_normal,n)
  self.mesh:set(1,gl.UT_normal,n)
  self.mesh:set(2,gl.UT_normal,n)
  self.mesh:set(3,gl.UT_normal,n)

  self.mesh:set(0,gl.UT_position, Vec3(rect.x,rect.y,meshZ)+offset)
  self.mesh:set(1,gl.UT_position, Vec3(rect.x+rect.width,rect.y,meshZ)+offset)
  self.mesh:set(2,gl.UT_position, Vec3(rect.x+rect.width,rect.y+rect.height,meshZ)+offset)
  self.mesh:set(3,gl.UT_position, Vec3(rect.x,rect.y+rect.height,meshZ)+offset)

  self.mesh:set(0,gl.UT_vertexAttrib0, Vec3(0,0,mbZ))
  self.mesh:set(1,gl.UT_vertexAttrib0, Vec3(1,0,mbZ))
  self.mesh:set(2,gl.UT_vertexAttrib0, Vec3(1,1,mbZ))
  self.mesh:set(3,gl.UT_vertexAttrib0, Vec3(0,1,mbZ))
end

function Plane:xz(args)
  local rect = args.rect
  local meshY = args.meshOffset
  local mbY = args.metaOffset
  local offset = args.offset

  local p0 = Vec3(rect.x,meshY,rect.y)+offset
  local p1 = Vec3(rect.x+rect.width,meshY,rect.y)+offset
  local p2 = Vec3(rect.x+rect.width,meshY,rect.y+rect.height)+offset
  local v0 = p1 - p0
  local v1 = p2-p0
  local n = lost.math.cross(v0,v1)
  lost.math.normalise(n)

  self.mesh:set(0,gl.UT_normal,n)
  self.mesh:set(1,gl.UT_normal,n)
  self.mesh:set(2,gl.UT_normal,n)
  self.mesh:set(3,gl.UT_normal,n)
  
  self.mesh:set(0,gl.UT_position, Vec3(rect.x,meshY,rect.y)+offset)
  self.mesh:set(1,gl.UT_position, Vec3(rect.x+rect.width,meshY,rect.y)+offset)
  self.mesh:set(2,gl.UT_position, Vec3(rect.x+rect.width,meshY,rect.y+rect.height)+offset)
  self.mesh:set(3,gl.UT_position, Vec3(rect.x,meshY,rect.y+rect.height)+offset)

  self.mesh:set(0,gl.UT_vertexAttrib0, Vec3(0,mbY,0))
  self.mesh:set(1,gl.UT_vertexAttrib0, Vec3(1,mbY,0))
  self.mesh:set(2,gl.UT_vertexAttrib0, Vec3(1,mbY,1))
  self.mesh:set(3,gl.UT_vertexAttrib0, Vec3(0,mbY,1))
end

function Plane:zy(args)
  local rect = args.rect
  local meshX = args.meshOffset
  local mbX = args.metaOffset
  local offset = args.offset

  local p0 = Vec3(meshX,rect.y,rect.x)+offset
  local p1 = Vec3(meshX,rect.y,rect.x+rect.width)+offset
  local p2 = Vec3(meshX,rect.y+rect.height,rect.x+rect.width)+offset
  local v0 = p1 - p0
  local v1 = p2-p0
  local n = lost.math.cross(v0,v1)
  lost.math.normalise(n)

  self.mesh:set(0,gl.UT_normal,n)
  self.mesh:set(1,gl.UT_normal,n)
  self.mesh:set(2,gl.UT_normal,n)
  self.mesh:set(3,gl.UT_normal,n)
  
  self.mesh:set(0,gl.UT_position, Vec3(meshX,rect.y,rect.x)+offset)
  self.mesh:set(1,gl.UT_position, Vec3(meshX,rect.y,rect.x+rect.width)+offset)
  self.mesh:set(2,gl.UT_position, Vec3(meshX,rect.y+rect.height,rect.x+rect.width)+offset)
  self.mesh:set(3,gl.UT_position, Vec3(meshX,rect.y+rect.height,rect.x)+offset)

  self.mesh:set(0,gl.UT_vertexAttrib0, Vec3(mbX,0,0))
  self.mesh:set(1,gl.UT_vertexAttrib0, Vec3(mbX,0,1))
  self.mesh:set(2,gl.UT_vertexAttrib0, Vec3(mbX,1,1))
  self.mesh:set(3,gl.UT_vertexAttrib0, Vec3(mbX,1,0))
end

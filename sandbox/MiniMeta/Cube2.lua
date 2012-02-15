module("meta",package.seeall)

require("lost.common.Class")
require("Plane")

local Vec3 = lost.math.Vec3
local Rect = lost.math.Rect

lost.common.Class "meta.Cube" {}
-- Params
-- size : size of the cube in one direction
-- numPlanes : number of planes in one direction
-- color : will be applied to all planes

function Cube:constructor(args)
  self.size = args.size
  self.numPlanes = args.numPlanes
  self.color = args.color
  self.renderNode = lost.rg.Node.create()

  local layout = lost.gl.BufferLayout()
  layout:add(gl.ET_vec3_f32, gl.UT_position)
  layout:add(gl.ET_vec3_f32, gl.UT_normal)
  layout:add(gl.ET_vec3_f32, gl.UT_vertexAttrib0)
  self.mesh = lost.mesh.Mesh.create(layout, gl.ET_u16)
  self.mesh.indexBuffer.drawMode = gl.GL_TRIANGLES
  local numVerts = 4 * self.numPlanes*3
  local numTris = 2 * self.numPlanes*3
  local numIndicesPerTri = 3
  local numIndices = numTris * numIndicesPerTri
  self.mesh:resetSize(numVerts, numIndices)
  self.renderNode:add(lost.rg.Draw.create(self.mesh))
  self.vertexOffset = 0
  self.indexOffset = 0
  
  -- init mesh
  self:createXY(args)
  self:createXZ(args)
  self:createZY(args)

  self.mesh.material.uniforms = lost.gl.UniformBlock.create()
  self.mesh.material:blendOff()
  self.mesh.material.cull = false
  self.mesh.material.color = args.color
  self.mesh.material.shader = metaShader

end

function Cube:createXY(args) self:createPlanes("xy",args) end
function Cube:createXZ(args) self:createPlanes("xz",args) end
function Cube:createZY(args) self:createPlanes("zy",args) end
function Cube:createPlanes(mode,args)
  local spaceStep=self.size/(self.numPlanes-1)
  local metaStep=1/(self.numPlanes-1)
  local curSpace = 0
  local curMeta = 0
  local off = -(self.size/2)
  local globalOffset = Vec3(off,off,off)
  for i=1,self.numPlanes do
    self:createPlane{
        ["mode"]=mode,
        color=self.color,
        rect=Rect(0,0,self.size,self.size), -- must stay at 0,0, should really be a Vec2
        offset = globalOffset,
        meshOffset=curSpace,
        metaOffset=curMeta}
    curSpace = curSpace + spaceStep
    curMeta = curMeta + metaStep
    self.vertexOffset = self.vertexOffset + 4
    self.indexOffset = self.indexOffset + 6
  end
end

function Cube:createPlane(args)
  if args.mode == "xy" then self:xy(args)
  elseif args.mode == "xz" then self:xz(args)
  elseif args.mode == "zy" then self:zy(args)
  end  
  local ioff = self.indexOffset
  local voff = self.vertexOffset
  self.mesh:setU16(ioff+0,gl.UT_index, voff+0)
  self.mesh:setU16(ioff+1,gl.UT_index, voff+1)
  self.mesh:setU16(ioff+2,gl.UT_index, voff+2)
  self.mesh:setU16(ioff+3,gl.UT_index, voff+2)
  self.mesh:setU16(ioff+4,gl.UT_index, voff+3)
  self.mesh:setU16(ioff+5,gl.UT_index, voff+0)  
end

function Cube:xy(args)
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
  local voff = self.vertexOffset
  self.mesh:set(voff+0,gl.UT_normal,n)
  self.mesh:set(voff+1,gl.UT_normal,n)
  self.mesh:set(voff+2,gl.UT_normal,n)
  self.mesh:set(voff+3,gl.UT_normal,n)

  self.mesh:set(voff+0,gl.UT_position, Vec3(rect.x,rect.y,meshZ)+offset)
  self.mesh:set(voff+1,gl.UT_position, Vec3(rect.x+rect.width,rect.y,meshZ)+offset)
  self.mesh:set(voff+2,gl.UT_position, Vec3(rect.x+rect.width,rect.y+rect.height,meshZ)+offset)
  self.mesh:set(voff+3,gl.UT_position, Vec3(rect.x,rect.y+rect.height,meshZ)+offset)

  self.mesh:set(voff+0,gl.UT_vertexAttrib0, Vec3(0,0,mbZ))
  self.mesh:set(voff+1,gl.UT_vertexAttrib0, Vec3(1,0,mbZ))
  self.mesh:set(voff+2,gl.UT_vertexAttrib0, Vec3(1,1,mbZ))
  self.mesh:set(voff+3,gl.UT_vertexAttrib0, Vec3(0,1,mbZ))
end

function Cube:xz(args)
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
  local voff = self.vertexOffset

  self.mesh:set(voff+0,gl.UT_normal,n)
  self.mesh:set(voff+1,gl.UT_normal,n)
  self.mesh:set(voff+2,gl.UT_normal,n)
  self.mesh:set(voff+3,gl.UT_normal,n)
  
  self.mesh:set(voff+0,gl.UT_position, Vec3(rect.x,meshY,rect.y)+offset)
  self.mesh:set(voff+1,gl.UT_position, Vec3(rect.x+rect.width,meshY,rect.y)+offset)
  self.mesh:set(voff+2,gl.UT_position, Vec3(rect.x+rect.width,meshY,rect.y+rect.height)+offset)
  self.mesh:set(voff+3,gl.UT_position, Vec3(rect.x,meshY,rect.y+rect.height)+offset)

  self.mesh:set(voff+0,gl.UT_vertexAttrib0, Vec3(0,mbY,0))
  self.mesh:set(voff+1,gl.UT_vertexAttrib0, Vec3(1,mbY,0))
  self.mesh:set(voff+2,gl.UT_vertexAttrib0, Vec3(1,mbY,1))
  self.mesh:set(voff+3,gl.UT_vertexAttrib0, Vec3(0,mbY,1))
end

function Cube:zy(args)
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
  local voff = self.vertexOffset

  self.mesh:set(voff+0,gl.UT_normal,n)
  self.mesh:set(voff+1,gl.UT_normal,n)
  self.mesh:set(voff+2,gl.UT_normal,n)
  self.mesh:set(voff+3,gl.UT_normal,n)
  
  self.mesh:set(voff+0,gl.UT_position, Vec3(meshX,rect.y,rect.x)+offset)
  self.mesh:set(voff+1,gl.UT_position, Vec3(meshX,rect.y,rect.x+rect.width)+offset)
  self.mesh:set(voff+2,gl.UT_position, Vec3(meshX,rect.y+rect.height,rect.x+rect.width)+offset)
  self.mesh:set(voff+3,gl.UT_position, Vec3(meshX,rect.y+rect.height,rect.x)+offset)

  self.mesh:set(voff+0,gl.UT_vertexAttrib0, Vec3(mbX,0,0))
  self.mesh:set(voff+1,gl.UT_vertexAttrib0, Vec3(mbX,0,1))
  self.mesh:set(voff+2,gl.UT_vertexAttrib0, Vec3(mbX,1,1))
  self.mesh:set(voff+3,gl.UT_vertexAttrib0, Vec3(mbX,1,0))
end

function Cube:transform(tf)
  self.mesh.transform = tf
end
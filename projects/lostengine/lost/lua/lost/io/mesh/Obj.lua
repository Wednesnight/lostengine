-- lost.io.mesh.Obj
module("lost.io", package.seeall)

require("lost.io.Loader")

function Loader:Obj(definition)
  local filename = definition["filename"] -- path to OBJ file to load
  if filename == nil then
    error("filename is mandatory for mesh:Obj")
  end
  local filedata = self.loader:load(filename)
  local objmesh = lost.mesh.Loader.obj(filedata)
  if definition["material"] ~= nil then
    local mat = definition["material"]
    for k,v in pairs(mat) do
      objmesh.material[k] = v
    end
	end  
	return objmesh
end
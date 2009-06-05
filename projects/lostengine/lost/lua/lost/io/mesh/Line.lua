-- lost.io.mesh.Line
module("lost.io", package.seeall)

require("lost.io.Loader")

-- create a line by specifying start/end Vec2
function Loader:Line2D(definition)
  local pointStart = definition["pointStart"]
  local pointEnd = definition["pointEnd"]
  
  result = lost.mesh.Line2D(pointStart, pointEnd)

  if definition["material"] ~= nil then
    local mat = definition["material"]
    for k,v in pairs(mat) do
      result.material[k] = v
    end
	end

  return result
end

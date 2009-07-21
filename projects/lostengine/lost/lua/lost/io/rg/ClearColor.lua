module("lost.io", package.seeall)

require("lost.io.Loader")

-- calls the glClear command with the given mask
function Loader:ClearColor(definition)
  local color = definition["color"]
  if color == nil then
    error("color is required for rg:ClearColor")
  end
  local result = lost.rg.ClearColor.create(color)
  local active = definition["active"]
  local name = definition["name"]
  if active ~= nil then
    result.active = active
  end
  if name ~= nil then
    result.name = name
  end
  return result
end
module("lost.io", package.seeall)

require("lost.io.Loader")

-- calls the glClear command with the given mask
function Loader:Clear(definition)
  local mask = definition["mask"]
  if mask == nil then
    error("mask is required for rg:Clear")
  end
  local result = lost.rg.Clear.create(definition["mask"])
  
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
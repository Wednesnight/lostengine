local rg =
{
  {lost.rg.Node},
  {lost.rg.Draw, {lost.mesh.Mesh.create()}}
}

local scene = lost.rg.Node.create()

for k,def in pairs(rg) do
  local instance = nil
  if type(def[2]) == "table" then
    instance = def[1].create(unpack(def[2]))
  else
    instance = def[1].create()
  end
  scene:add(instance)
end

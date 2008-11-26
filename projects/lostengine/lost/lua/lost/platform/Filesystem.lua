module("lost.platform", package.seeall)

--[[
     Filesystem utility functions
  ]]

function fileExists(filename)
  local file = io.open(filename)
  local result = (file ~= nil)
  if result then
    file:close()
  end
  return result
end

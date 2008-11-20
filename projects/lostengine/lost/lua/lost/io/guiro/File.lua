module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: File
  ]]

require("lost.io.Loader")

function Loader:File(filename)
  return self.loader:load(filename)
end

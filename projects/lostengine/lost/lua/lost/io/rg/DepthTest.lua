module("lost.io", package.seeall)

require("lost.io.Loader")

function Loader:DepthTest(enable)
  return lost.rg.DepthTest.create(enable)
end
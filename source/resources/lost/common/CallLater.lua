--[[
    lost.common.CallLater
  ]]
module("lost.common", package.seeall)

function callLater(func, obj)
  tasklet.updateQueue:queue(func, obj)
end

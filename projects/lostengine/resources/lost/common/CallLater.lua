--[[
    lost.common.CallLater
  ]]
module("lost.common", package.seeall)

using "lost.application.currentTasklet"

function callLater(func, obj)
  if currentTasklet ~= nil then
    currentTasklet.updateQueue:queue(func, obj)
  end
end

-- lost.common.CallLater
-- this is deliberately global since the tasklet will call the process
-- function if it is present in the global namespace
-- you also don't have to type lost.comm.CallLater.callLater(myFunc) all the time.

callLaterQueue = {}
callLaterCheck = {} -- functions are keys here so we can check quickly if a function was already queued

function callLater(func)
  if callLaterCheck[func] == nil then
    callLaterQueue[#callLaterQueue+1] = func
    callLaterCheck[func] = true
  end
end

function processCallLaterQueue()
  local num = 0
  for k, func in pairs(callLaterQueue) do
    func()
    callLaterQueue[k] = nil
    callLaterCheck[func] = nil
    num = num +1
  end
  if num > 0 then
    log.debug("processed "..num.." functions in callLaterQueue")
  end
end
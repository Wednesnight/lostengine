-- lost.common.CallLater
-- this is deliberately global since the tasklet will call the process
-- function if it is present in the global namespace
-- you also don't have to type lost.comm.CallLater.callLater(myFunc) all the time.

callLaterQueue = {}

function callLater(func)
  callLaterQueue[func] = true
end

function processCallLaterQueue()
  local num = 0
  for func, v in pairs(callLaterQueue) do
    func()
    callLaterQueue[func] = nil
    num = num +1
  end
  if num > 0 then
    log.debug("processed "..num.." functions in callLaterQueue")
  end
end
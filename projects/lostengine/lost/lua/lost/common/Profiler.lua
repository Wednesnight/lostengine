module("lost.common", package.seeall)

--[[
     Profiler
  ]]
class "lost.common.Profiler"
Profiler = _G["lost.common.Profiler"]

function Profiler:__init()
  self.calls = {}
end

function Profiler:enable()
  debug.sethook(function(which) self:call(debug.getinfo(2)) end, "c")
end

function Profiler:disable()
  debug.sethook()
end

function Profiler:call(info)
  if info.short_src ~= "[C]" then
    info.source = info.source or ""
    info.short_src = info.short_src or ""
    if lost.globals.state then
      info.short_src = lost.globals.state:getScriptFilename(info.source, info.short_src)
    end
    info.linedefined = info.linedefined or ""
    info.name = info.name or ""
    local key = info.short_src .." [".. info.linedefined .."] ".. info.name
    if not self.calls[key] then
      self.calls[key] = 1
    else
      self.calls[key] = self.calls[key] + 1
    end
  end
end

function Profiler:report()
  log.info("Profiler report:")
  for name,count in next,self.calls do
    log.info("  ".. name ..": ".. count)
  end
end

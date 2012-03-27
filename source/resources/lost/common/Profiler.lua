module("lost.common", package.seeall)

require("lost.common.Class")

--[[
     Profiler
  ]]
lost.common.Class "lost.common.Profiler" {}

function Profiler:constructor()
  self.calls = {}
end

function Profiler:enable()
  debug.sethook(function(which) self:call(debug.getinfo(2)) end, "c")
  self.start = os.clock()
end

function Profiler:disable()
  debug.sethook()
  self.duration = os.clock() - self.start
end

function Profiler:call(info)
  if info.short_src ~= "[C]" then
    info.source = info.source or ""
    info.short_src = info.short_src or ""
    info.short_src = tasklet.lua:getScriptFilename(info.source, info.short_src)
    info.linedefined = info.linedefined or ""
    info.name = info.name or ""
    local source
    if string.len(info.short_src) > 0 then
      source = info.short_src
    else
      source = "unknown"
    end
    local name
    if (string.len(info.name) > 0) and (info.name ~= "?") then
      name = info.name .." "
    else
      name = ""
    end
    name = name .."(line ".. info.linedefined ..")"
    if not self.calls[source] then
      self.calls[source] = {}
    end
    if not self.calls[source][name] then
      self.calls[source][name] = 1
    else
      self.calls[source][name] = self.calls[source][name] + 1
    end
  end
end

function Profiler:report()
  log.info("Profiler report:")
  table.sort(self.calls)
  for source,names in next,self.calls do
    log.info("  ".. source ..":")
    table.sort(names)
    for name,count in next,names do
      log.info("    ".. name ..":")
      log.info("      calls    : ".. count)
      log.info("      calls/sec: ".. math.floor(count / self.duration))
    end
  end
end

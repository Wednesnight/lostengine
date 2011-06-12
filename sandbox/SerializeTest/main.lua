require("lost.common.PrefsManager")

function startup()
  prefsManager = lost.common.PrefsManager{prefix="LostEngine/SerializeTest",filename="prefs.lua"}
  prefs = prefsManager:load() or {}
  log.debug("-- prefs")
  for k,v in pairs(prefs) do
    print(tostring(k).." = "..tostring(v))
  end
end

function shutdown()
  if not prefs.count then
    prefs.count = 0
  else
    prefs.count = prefs.count + 1
  end
  prefsManager:save(prefs)
end

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end
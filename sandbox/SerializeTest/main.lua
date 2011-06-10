require("lost.common.Serializer")

t = 
{
  color = 
  {
    1,1,1
  },
  alpha = 1,
  name = "Hello",
}

function startup()
  log.debug("--")
  local s = lost.common.Serializer()
  log.debug(s:serialize(t))
  
  local userDataPath = lost.platform.getUserDataPath()
  log.debug(tostring(userDataPath))
  log.debug("userDataPath exists: "..tostring(boost.filesystem.exists(userDataPath)))
  local appDataPath = userDataPath / "LostEngine" / "SerializeTest"
  log.debug(tostring(appDataPath))
  log.debug("appDataPath exists: "..tostring(boost.filesystem.exists(appDataPath)))
  log.debug("creating appDataPath: "..tostring(boost.filesystem.create_directories(appDataPath)))
end

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end
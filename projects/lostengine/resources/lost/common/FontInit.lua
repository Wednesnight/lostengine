log.debug("-------------------")
log.debug("-------------------")

local t = require("lost/fontconfig")

for k,v in pairs(t) do
--  log.debug(k.." -> "..v)
  tasklet.fontManager:addEntry(k,v)
end

log.debug("-------------------")
log.debug("-------------------")



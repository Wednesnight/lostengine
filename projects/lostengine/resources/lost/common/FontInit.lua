local t = require("lost/fontconfig")
for k,v in pairs(t) do
  tasklet.fontManager:addEntry(k,v)
end


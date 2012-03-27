-- lost.declarative.Bitmap
module("lost.declarative", package.seeall)

lost.common.Class "lost.declarative.Bitmap" {}

function Bitmap:constructor(loader)
  self.loader = loader
end

function Bitmap:Bitmap(def)
  if def.filename == nil then
    error("filename is required bitmap:Bitmap", 2)
  end
  local data = self.loader:load(def.filename)
  local bitmap = lost.bitmap.Bitmap(data)
--  if def.x then
--    bitmap.x = def.x
--  end
  return bitmap
end

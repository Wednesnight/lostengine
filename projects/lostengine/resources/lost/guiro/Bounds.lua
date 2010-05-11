module("lost.guiro", package.seeall)

require("lost.common.Class")
require("lost.guiro.OldLayoutFunctions")

lost.common.Class "lost.guiro.Bounds" {}

local constructParamErrorMsg = "Bounds must be constructed with one table or 4 functions"
local errorLevel = 3

-- either one prameter, a table
-- or 4 functions
function Bounds:constructor(...)
--  log.debug("argn "..tostring(arg.n))
  local x,y,width, height = nil

  local argsAreFuncs = true
  for k,v in ipairs(arg) do
    if type(v) ~= "function" then
      argsAreFuncs = false
      break
    end
  end

  if argsAreFuncs then
--    log.debug("--- CONSTRUCT ... CLASSIC")
    x,y,width, height = unpack(arg)
  else
--    log.debug("--- CONSTRUCT ... NEW")
  end

  self.x = x
  self.y = y
  self.width = width
  self.height = height
end

function Bounds:rect(parentRect)
  return lost.math.Rect(math.floor(self:x(parentRect)),
                        math.floor(self:y(parentRect)),
                        math.floor(self:width(parentRect)),
                        math.floor(self:height(parentRect)))
end

-- the receiver is the target, source will be applied to it
-- values from source are only copied if they are ~= nil
function Bounds:merge(source)
  if source.x ~= nil then self.x = source.x end
  if source.y ~= nil then self.y = source.y end
  if source.width ~= nil then self.width = source.width end
  if source.height ~= nil then self.height = source.height end
end

-- returns true if all bounds parameters are ~= nil
function Bounds:complete()
  local result = true
  if (self.x == nil) or 
     (self.y == nil) or 
     (self.width == nil) or 
     (self.height == nil) then
    result = false
  end
  return result
end





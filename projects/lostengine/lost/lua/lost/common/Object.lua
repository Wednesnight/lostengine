module("lost.common", package.seeall)

require("lost.common.Class")
--[[
     Common base class for Lua classes that provides some more structure
  ]]
Object = lost.common.Class("lost.common.Object")

--[[ 
    class (not instance) based table holding the list of class names
    must be extended through View:addBase()
  ]]
Object.bases = { "lost.common.Object" }


function Object:__init()
--  log.debug("initializing object of type: "..self:className())
end
--[[ 
    adds className to class.bases
    a derived class MUST use its direct base to call this method because class.bases is extended from self.bases
  ]]
function Object:addBase(clazz, className)
  clazz.bases = {}
  for k,v in next,self.bases do
    clazz.bases[k] = v
  end
  clazz.addBase = self.addBase
  table.insert(clazz.bases, className)
end

--[[ 
    checks if self is a or derived from className
  ]]
function Object:is(className)
  local result = self.bases
  if result then
    for k,base in next,self.bases do
      result = base == className
      if result then
        break
      end
    end
  end
  return result
end

--[[ 
    returns the last entry in self.bases
  ]]
function Object:className()
  local result = "nil"
  if self.bases then
    result = self.bases[table.maxn(self.bases)]
  end
  return result
end

--[[ 
    prints self.bases hierarchy
  ]]
function Object:printBases()
  local prefix = ""
  for k,v in next,self.bases do
    log.debug(prefix .."|-- ".. v)
    prefix = prefix .."    "
  end
end

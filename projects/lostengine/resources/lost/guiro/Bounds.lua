module("lost.guiro", package.seeall)

require("lost.common.Class")
require("lost.guiro.OldLayoutFunctions")
require("lost.guiro.LayoutFunctions")

lost.common.Class "lost.guiro.Bounds" {}

-- all builtin layout functions known to Bounds
Bounds.builtins = {xleft, xcenter, xright, xrel, xabs, 
                  ybottom, ycenter, ytop, yrel, yabs,
                  wabs, wrel, wfit,
                  habs, hrel, hfit
                 }

-- default layout functions for the positions in the parameter table passed to bounds
-- these are defined in the order of appearance in the resulting rect (x,y,w,h)
Bounds.defaults = {xpos,ypos,wsize,hsize}

-- aliases map common layout functions from the simples name of that context onto a layout function
-- Example:
-- xleft was the old layout function for aligning the x-coordinate to the parents left side
-- "x" is redundant since it is specified in the x-position of the rect anyway
-- so when we use a string, we can just right "left" at the x-position, and it is looked up, knowing that 
-- "left" must be an x-alignment since it was written at position 1 of the incoming parameter table.
Bounds.aliases = {
  {left=xl, center=xc, right=xr}, -- index 1 = x alignments
  {top=yt, center=yc, bottom=yb}, -- index 2 = y alignments
  {},
  {}
}

-- checks if a function is a known builtin layout function
function Bounds.isBuiltin(f)
  result = false
  assert(type(f)=="function", "only functions can be builtins")
  for k,v in pairs(Bounds.builtins) do
    if v == f then 
      result = true
      break
    end
  end
  return result
end

-- tries to figure out what the parameters passed inside "entry" are meant to be for the given index
-- index specifies which component of the resulting rect this entry is supposed to be
-- 1 = x, 2 = y, 3 = w, 4 = h
function Bounds.decodeEntry(index, entry)
  assert((index>=1 and index <=4), "entry index must be in range [1,4]")
  local result = nil
  
  -- function
  if type(entry) == "function" then
    if Bounds.isBuiltin(entry) then
      result = entry() -- builtins must be called to build the default functions
    else
      result = entry -- other functions are copied verbatim
    end
  end
  
  -- number
  if type(entry) == "number" then
    -- absolute value, find default function and pass it in 
    result = Bounds.defaults[index]({abs=entry})
  end

  -- string
  if type(entry) == "string" then
    -- relative value, find default function and pass it in, OR builtin function
    local builtin = Bounds.aliases[index][entry]
    if builtin then
      result = builtin()
    else
      result = Bounds.defaults[index]({rel=tonumber(entry)})
    end
  end
  
  -- table, contains abs and/or rel values, pass it in to default function
  -- if any of the parameters is a bultin function, it will override the default
  local builtin = nil
  if type(entry) == "table" then
    log.debug("-- decoding TABLE with length "..tostring(#entry))
    local t = {}
    for k,v in ipairs(entry) do
      log.debug("-- decoding "..tostring(k).." "..tostring(v))
      if type(v) == "string" then
        local bi = Bounds.aliases[index][v]
        if bi == nil then
          t.rel = tonumber(v)
        else
          builtin = bi
        end
      elseif type(v) == "number" then 
        t.abs = v
      end
    end

    for k,v in pairs(t) do -- DEBUG
      log.debug("-- RESULT: "..tostring(k).." "..tostring(v))
    end -- DEBUG

    if builtin then
      result = builtin(t)
    else
      result = Bounds.defaults[index](t)
    end
  end
  
  return result
end

function Bounds.decode(t)
  return Bounds.decodeEntry(1,t[1]),
         Bounds.decodeEntry(2,t[2]),
         Bounds.decodeEntry(3,t[3]),
         Bounds.decodeEntry(4,t[4])
end

function Bounds.argsAreFunctions(args)
  local result = true
  for k,v in ipairs(args) do
    if type(v) ~= "function" then
      result = false
      break
    end
  end
  return result
end

-- either one prameter, a table
-- or 4 functions
function Bounds:constructor(...)
  self.x,self.y,self.width,self.height = Bounds.decode(arg)


--  self.x = Bounds.decodeEntry(1,arg[1])
--  self.y = Bounds.decodeEntry(2,arg[2])
--  self.width = Bounds.decodeEntry(3,arg[3])
--  self.height = Bounds.decodeEntry(4,arg[4])
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





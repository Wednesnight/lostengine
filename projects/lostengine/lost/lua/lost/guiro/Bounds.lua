module("lost.guiro", package.seeall)

require("lost.common.Class")

--[[
     Bounds class
  ]]
lost.common.Class "lost.guiro.Bounds" {}

--[[
  choose the functions below to build a bounds object
  all values are functions!
  use rect to conert a bounds object to a lost.math.Rect
  
  example: 
  local b = lost.guiro.Bounds
  local mybounds = b.Bounds(b.xabs(0), b.xabs(0))
]]
function Bounds:constructor(x, y, width, height)
  self.x = x
  self.y = y
  self.width = width
  self.height = height
end

function Bounds:rect(parentRect)
  return lost.math.Rect(self:x(parentRect), self:y(parentRect), self:width(parentRect), self:height(parentRect))
end

function xcenter(mod)
  mod = mod or {}
  mod.abs = mod.abs or 0
  mod.rel = mod.rel or .5
  return function(self, parentRect) return parentRect.x + ((parentRect.width - self:width(parentRect)) * mod.rel) + mod.abs end
end

function xleft(mod)
  mod = mod or {}
  mod.abs = mod.abs or 0
  mod.rel = mod.rel or 0
  return function(self, parentRect) return parentRect.x + (mod.abs + (parentRect.width * mod.rel)) end
end

function xright(mod)
  mod = mod or {}
  mod.abs = mod.abs or 0
  mod.rel = mod.rel or 0
  return function(self, parentRect) return (parentRect:maxX() - self:width(parentRect) + 1) + (mod.abs + (parentRect.width * mod.rel)) end
end

function xabs(x, mod)
  mod = mod or 0
  return function(self, parentRect) 
    return (parentRect.x + x) + (parentRect.width * mod)
  end
end

function xrel(x, mod)
  mod = mod or 0
  return function(self, parentRect)
    return (parentRect.x + (parentRect.width * x)) + mod
  end
end

function ycenter(mod)
  mod = mod or {}
  mod.abs = mod.abs or 0
  mod.rel = mod.rel or .5
  return function(self, parentRect) return parentRect.y + ((parentRect.height - self:height(parentRect)) * mod.rel) + mod.abs end
end

function ybottom(mod)
  mod = mod or {}
  mod.abs = mod.abs or 0
  mod.rel = mod.rel or 0
  return function(self, parentRect) return parentRect.y + (mod.abs + (parentRect.height * mod.rel)) end
end

function ytop(mod)
  mod = mod or {}
  mod.abs = mod.abs or 0
  mod.rel = mod.rel or 0
  return function(self, parentRect) return (parentRect:maxY() - self:height(parentRect) + 1) + (mod.abs + (parentRect.height * mod.rel)) end
end

function yabs(y, mod)
  mod = mod or 0
  return function(self, parentRect)
    return (parentRect.y + y) + (parentRect.width * mod)
  end
end

function yrel(y, mod)
  mod = mod or 0
  return function(self, parentRect)
    return (parentRect.y + (parentRect.height * y)) + mod
  end
end

function wabs(width, mod)
  mod = mod or 0
  return function(self, parentRect)
    return width + (parentRect.width * mod)
  end
end

function wrel(width, mod)
  mod = mod or 0
  return function(self, parentRect)
    return (parentRect.width * width) + mod
  end
end

function habs(height, mod)
  mod = mod or 0
  return function(self, parentRect)
    return height + (parentRect.height * mod)
  end
end

function hrel(height, mod)
  mod = mod or 0
  return function(self, parentRect)
    return (parentRect.height * height) + mod
  end
end

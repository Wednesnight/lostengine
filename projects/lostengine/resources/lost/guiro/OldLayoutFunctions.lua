module("lost.guiro", package.seeall)

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

function xabs(abs, rel)
  if type(abs) == "table" then -- compatibility for new calling conventions that require tabel as sole parameter
    abs = abs.abs or 0
    rel = abs.rel or 0
  else
    rel = rel or 0
  end
  return function(self, parentRect) return (parentRect.x + abs) + (parentRect.width * rel) end
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

function wfit()
  return wrel(1)
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

function hfit()
  return hrel(1)
end
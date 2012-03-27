module("lost.guiro", package.seeall)

function xpos(t)
  t = t or {}
  local abs = t.abs or 0
  local rel = t.rel or 0
  return function(self, parentRect) 
    return (parentRect.x + abs) + (parentRect.width * rel)
  end  
end

function ypos(t)
  t = t or {}
  local abs = t.abs or 0
  local rel = t.rel or 0
  return function(self, parentRect)
    return (parentRect.y + abs) + (parentRect.width * rel)
  end
end

function hsize(t)
  t = t or {}
  local abs = t.abs or 0
  local rel = t.rel or 0
  return function(self, parentRect)
    return (parentRect.height * rel) + abs
  end
end

function wsize(t)
  t = t or {}
  local abs = t.abs or 0
  local rel = t.rel or 0
  return function(self, parentRect)
    return (parentRect.width * rel) + abs
  end
end

function xr(t)
  t = t or {}
  local abs = t.abs or 0
  local rel = t.rel or 0
  return function(self, parentRect)
    return (parentRect:maxX() - self:width(parentRect) + 1) + (abs + (parentRect.width * rel))
  end
end

function xl(t)
  t = t or {}
  local abs = t.abs or 0
  local rel = t.rel or 0
  return function(self, parentRect)
    return parentRect.x + (abs + (parentRect.width * rel))
  end
end

function xc(t)
  t = t or {}
  local abs = t.abs or 0
  local rel = t.rel or .5
  return function(self, parentRect)
    return parentRect.x + ((parentRect.width - self:width(parentRect)) * rel) + abs 
  end
end

function yc(t)
  t = t or {}
  local abs = t.abs or 0
  local rel = t.rel or .5
  return function(self, parentRect) 
    return parentRect.y + ((parentRect.height - self:height(parentRect)) * rel) + abs
  end
end

function yb(t)
  t = t or {}
  local abs = t.abs or 0
  local rel = t.rel or 0
  return function(self, parentRect)
    return parentRect.y + (abs + (parentRect.height * rel))
  end
end

function yt(t)
  t = t or {}
  local abs = t.abs or 0
  local rel = t.rel or 0
  return function(self, parentRect) 
    return (parentRect:maxY() - self:height(parentRect) + 1) + (abs + (parentRect.height * rel))
  end
end



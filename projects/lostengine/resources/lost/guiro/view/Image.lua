module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.Image" "lost.guiro.view.View" {}

-- imageLayer needs to be created and set by style
function Image:constructor(args)
  lost.guiro.view.View.constructor(self, args)
  local t = args or {}
  self.id = t.id or "image"
  if t.bitmap then self.imageLayer:bitmap(t.bitmap)
  elseif t.texture then self.imageLayer:texture(t.texture) end
  self:scale(t.scale or "none")
  self:filter(t.filter or false)
  self:flip(t.flip or false)
	self._bbcounter = lost.profiler.BBCount("lost.guiro.view.Image")          
end

function Image:bitmap(v)
  self.imageLayer:bitmap(v)
end

function Image:texture(v)
  self.imageLayer:texture(v)
end

function Image:scale(...)
  if arg.n >= 1 then
    self.imageLayer:scale(arg[1])
  else
    return self.imageLayer:scale()
  end
end

function Image:filter(...)
  if arg.n >= 1 then
    self.imageLayer:filter(arg[1])
  else
    return self.imageLayer:filter()
  end
end

function Image:flip(...)
  if arg.n >= 1 then
    self.imageLayer:flip(arg[1])
  else
    return self.imageLayer:flip()
  end
end



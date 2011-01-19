module("lost.guiro.view", package.seeall)

require("lost.guiro.view.View")

lost.common.Class "lost.guiro.view.Label" "lost.guiro.view.View" {}

local Color = lost.common.Color
local Vec2 = lost.math.Vec2

function Label:constructor(args)
  lost.guiro.view.View.constructor(self, args)
  local t = args or {}
  self.id = t.id or "label"
  self.textLayer = self.layer:recursiveFindById("text")
  self.textLayer:font(t.font or {"Vera", 12})
  self.textLayer:text(t.text or "")
  self.textLayer:color(t.color or Color(0,0,0))
  self.textLayer:shadow(t.shadow or false)
  self.textLayer:shadowOffset(t.shadowOffset or Vec2(1,1))
  self.textLayer:shadowColor(t.shadowColor or Color(0,0,0,.3))
  self.textLayer:valign(t.valign or "center")
  self.textLayer:halign(t.halign or "center")
end

function Label:text(...)
  if arg.n >= 1 then 
    self.textLayer:text(arg[1])
  else
    if self.textLayer then
      return self.textLayer:text()
    else
      return nil
    end
  end
end

function Label:font(...)
  if arg.n >=1 then
    self.textLayer:font(arg[1])
  else
    return self.textLayer._font
  end
end

function Label:color(...)
  if arg.n >=1 then
    self.textLayer:color(arg[1])
  else
    return self.textLayer:color()
  end
end

function Label:valign(...)
  if arg.n >=1 then
    self.textLayer:valign(arg[1])
  else
    return self.textLayer:valign()
  end
end

function Label:halign(...)
  if arg.n >=1 then
    self.textLayer:halign(arg[1])
  else
    return self.textLayer:halign()
  end
end

function Label:shadow(...)
  if arg.n >=1 then
    self.textLayer:shadow(arg[1])
  else
    return self.textLayer:shadow()
  end
end

function Label:shadowOffset(...)
  if arg.n >=1 then
    self.textLayer:shadowOffset(arg[1])
  else
    return self.textLayer:shadowOffset()
  end
end

function Label:shadowColor(...)
  if arg.n >=1 then
    self.textLayer:shadowColor(arg[1])
  else
    return self.textLayer:shadowColor()
  end
end




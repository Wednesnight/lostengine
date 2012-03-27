module("lost.guiro.layout", package.seeall)

require("lost.common.Class")
require("lost.guiro.layout.Layout")

lost.common.Class "lost.guiro.layout.Horizontal" "lost.guiro.layout.Layout" {}

function Horizontal:constructor(args)
  self.halign = args.halign or nil -- left, center, right, nil
  self.valign = args.valign or nil -- top, center, bottom, nil
  self.xoffset = args.xoffset or 0
  self.yoffset = args.yoffset or 0
  self.spacing = args.spacing or 0
end

function Horizontal:sumItemWidth(items)
  local result = 0
  for k,item in pairs(items) do
    result = result + item.rect.width
  end
  return result
end

function Horizontal:sumItemWidthWithSpacing(items)
  local result = self:sumItemWidth(items)
  local n = #items - 1
  if n < 0 then n = 0 end
  result = result + n * self.spacing
  return result
end

function Horizontal:apply(target, items)
  local xAlignOffset = 0
  if self.halign then
    local s = self:sumItemWidthWithSpacing(items)
    if self.halign == "left" then xAlignOffset=0
    elseif self.halign == "center" then xAlignOffset = math.floor((target.rect.width - s)/2)
    elseif self.halign == "right" then xAlignOffset = target.rect.width - s
    end
  end
  local offset = xAlignOffset + self.xoffset
  for k,item in ipairs(items) do
    item:x(offset)
    if self.valign then
      item:y({self.valign, self.yoffset})
    end
    offset = offset + item.rect.width + self.spacing
  end
end

module("lost.guiro.layout", package.seeall)

require("lost.common.Class")
require("lost.guiro.layout.Layout")

lost.common.Class "lost.guiro.layout.Vertical" "lost.guiro.layout.Layout" {}

function Vertical:constructor(args)
  self.halign = args.halign or nil -- left, center, right, nil
  self.valign = args.valign or nil -- top, center, bottom, nil
  self.xoffset = args.xoffset or 0
  self.yoffset = args.yoffset or 0
  self.spacing = args.spacing or 0
end

function Vertical:sumItemHeight(items)
  local result = 0
  for k,item in pairs(items) do
    result = result + item.rect.height
  end
  return result
end

function Vertical:sumItemHeightWithSpacing(items)
  local result = self:sumItemHeight(items)
  local n = #items - 1
  if n < 0 then n = 0 end
  result = result + n * self.spacing
  return result
end

function Vertical:apply(target, items)
  local yAlignOffset = 0
  if self.valign then
    local s = self:sumItemHeightWithSpacing(items)
    if self.valign == "bottom" then yAlignOffset=0
    elseif self.valign == "center" then yAlignOffset = math.floor((target.rect.height - s)/2)
    elseif self.valign == "top" then yAlignOffset = target.rect.height - s
    end
  end
  local offset = yAlignOffset + self.yoffset
  for k,item in ipairs(items) do
    item:y(offset)
    if self.halign then
      item:x({self.halign, self.xoffset})
    end
    offset = offset + item.rect.height + self.spacing
  end
end

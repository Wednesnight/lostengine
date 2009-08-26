-- lost.guiro.Label
module("lost.guiro", package.seeall)

require("lost.guiro.View")
require("lost.guiro.Bounds")

local Rect = lost.math.Rect
local Vec2 =lost.math.Vec2
local Vec3 =lost.math.Vec3
local MatrixTranslation =lost.math.MatrixTranslation

lost.common.Class "lost.guiro.Label" "lost.guiro.View" {}

-- render triggers
-- * text
-- * _fontSize
-- * font

-- layout triggers
-- * layout
-- * _shadowOffset

--[[  A Label displays one line of text inside it's bounds.
  Text is rendered with the given font and _fontSize.
  Optionally, a shadow can be drawn underneath the original text with a flexible offset
  and different color.
]]
function Label:constructor()
  lost.guiro.View.constructor(self)

  self._font = nil
  self._fontSize = 12
  self.textMesh = lost.font.RenderedText.create()
  self.shadowMesh = lost.font.RenderedText.create()
  self.textNode = lost.rg.Draw.create(self.textMesh)
  self.textNode.name = "drawText"
  self.shadowNode = lost.rg.Draw.create(self.shadowMesh)
  self.shadowNode.name = "drawShadow"
  self.renderNode:add(self.shadowNode)
  self.renderNode:add(self.textNode)

  -- _text is interpreted as UTF-8 character data
  self._text = nil -- label is empty by default since we can't load a default font from here and prerender something
  -- textColor is managed inside textMesh.material.color
  self.textMesh.material.color = lost.common.Color(0,0,0,1)
  self._shadowOffset = Vec2(1,-1)
  -- shadowColor is managed inside shadowMesh.material.color
  -- showShadow is managed as shadowNode.active
  self.shadowNode.active = false
  self._halign = "center" -- possible values: left, center, right
  self._valign = "center" -- possible values: top, center, bottom
  
  -- suppress background and frame by default
  self:showFrame(false)
  self:showBackground(false)
end

function Label:updateLayout(forceUpdate)
  local gr, lr = lost.guiro.View.updateLayout(self, forceUpdate)

  -- initially, the text is aligned to the lower left border
  -- if any of the alignment strings are borked, the text will still display,
  -- but the alignment will be only off inside the label's bounds
  local textPos = Vec3(gr.x, gr.y, 0)

  if self._halign == "center" then
    textPos.x = gr.x + (gr.width-self.textMesh.size.width)/2
  elseif self._halign == "left" then
    textPos.x = gr.x
  elseif self._halign == "right" then
    textPos.x = gr.x+gr.width - self.textMesh.size.width
  end

  if self._valign == "center" then
    textPos.y = gr.y+((gr.height-self.textMesh.size.height)/2)+(-1*self.textMesh.min.y)
  elseif self._valign == "top" then
    textPos.y = gr.y+gr.height-self.textMesh.size.height
  elseif self._valign == "bottom" then
    textPos.y = gr.y+(-1*self.textMesh.min.y)
  end

  local shadowPos = textPos + Vec3(self._shadowOffset.x, self._shadowOffset.y, 0)

  self.textMesh.transform = MatrixTranslation(textPos)
  self.shadowMesh.transform = MatrixTranslation(shadowPos)
  
  return gr, lr
end
-- call this after a param change to update the appearance of text and shadow Mesh
-- will recreate the mesh data (but not the meshes)
function Label:render()
  log.debug("render enter")
  if self._font == nil then
    log.warn("can't update Label because font is nil")
    return
  end
  log.debug("trying to render textMesh")
  self._font:render(self._text, self._fontSize, self.textMesh)
  log.debug("trying to render shadowMesh")
  self._font:render(self._text, self._fontSize, self.shadowMesh)
  self:updateLayout(false)
end

function Label:text(s)
  if s~= nil then
    self._text = s
    self:render()
  else
    return self._text
  end
end

function Label:font(v)
  if v ~= nil then
    self._font = v
    self:render()
  else
    return self._font
  end
end

function Label:fontSize(v)
  if v ~= nil then
    self._fontSize = v
    self:render()
  else
    return self._fontSize
  end
end

function Label:textColor(v)
  if v ~= nil then
    self.textMesh.material.color = v
  else
    return self.textMesh.material.color
  end
end

function Label:shadowColor(v)
  if v ~= nil then
    self.shadowMesh.material.color = v
  else
    return self.shadowMesh.material.color
  end
end

function Label:shadowOffset(v)
  if v ~= nil then
    self._shadowOffset = v
    self:updateLayout(false)
  else
    return self._shadowOffset
  end
end

function Label:showShadow(v)
  if v ~= nil then
    self.shadowNode.active = v
  else
    return self.shadowNode.active
  end
end

function Label:halign(v)
  if v ~= nil then
    self._halign = v
    self:updateLayout(false)
  else
    return self.__halign
  end
end

function Label:valign(v)
  if v ~= nil then
    self._valign = v
    self:updateLayout(false)
  else
    return self._valign
  end
end
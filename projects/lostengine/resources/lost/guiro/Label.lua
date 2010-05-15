module("lost.guiro", package.seeall)

require("lost.guiro.View")
require("lost.common.Shaders")

lost.common.Class "lost.guiro.Label" "lost.guiro.View" {}

-- local shortcuts
local Rect = lost.math.Rect
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local MatrixTranslation = lost.math.MatrixTranslation

--[[
    A Label displays one line of text inside it's bounds.
    Text is rendered with the given font and _fontSize.
    Optionally, a shadow can be drawn underneath the original text with a flexible offset and different color.
  ]]
function Label:constructor(textureManager)
  assert(textureManager)
  lost.guiro.View.constructor(self, textureManager)

  self._font = nil
  self._fontSize = 12
  self.textMesh = lost.font.RenderedText.create()
  self.textMesh.material.shader = lost.common.Shaders.textureShader()
  self.shadowMesh = lost.font.RenderedText.create()
  self.shadowMesh.material.shader = lost.common.Shaders.textureShader()
  self.textNode = lost.rg.Draw.create(self.textMesh)
  self.textNode.name = "drawText"
  self.shadowNode = lost.rg.Draw.create(self.shadowMesh)
  self.shadowNode.name = "drawShadow"
  self.renderNode:add(self.shadowNode)
  self.renderNode:add(self.textNode)

  -- _text is interpreted as UTF-8 character data
  self._text = "" -- label is empty by default since we can't load a default font from here and prerender something
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
  
  self._dirtyText = false -- guard to prevent continuous recreation of text upon resize
end

function Label:afterRedraw()
  if self._font == nil then
    log.warn("--------- NO FONT")
    return
  end

  if self._dirtyText then
    self._font:render(self._text, self._fontSize, self.textMesh)
    self._font:render(self._text, self._fontSize, self.shadowMesh)
    self:updateAlign()
    self._dirtyText = false
  end
end

function Label:afterLayout()
  lost.guiro.View.afterLayout(self)
  self:updateAlign()
end

function Label:updateAlign()
  -- initially, the text is aligned to the lower left border
  -- if any of the alignment strings are borked, the text will still display,
  -- but the alignment will be only off inside the label's bounds
  local textPos = Vec3(self.rect.x, self.rect.y, 0)

  if self._halign == "center" then
    textPos.x = self.rect.x + (self.rect.width-self.textMesh.size.width)/2
  elseif self._halign == "left" then
    textPos.x = self.rect.x
  elseif self._halign == "right" then
    textPos.x = self.rect.x+self.rect.width - self.textMesh.size.width
  end

  if self._valign == "center" then
--    textPos.y = self.rect.y+((self.rect.height-self.textMesh.size.height)/2)-self.textMesh.min.y
      textPos.y = self.rect.y+((self.rect.height-self.textMesh.fontHeight)/2)+math.abs(self.textMesh.fontDescender)
  elseif self._valign == "top" then
    textPos.y = self.rect.y+self.rect.height-self.textMesh.fontHeight
  elseif self._valign == "bottom" then
    textPos.y = self.rect.y+(-1*self.textMesh.fontDescender)
  end
  
  textPos.x = math.floor(textPos.x)
  textPos.y = math.floor(textPos.y)

  local shadowPos = textPos + Vec3(self._shadowOffset.x, self._shadowOffset.y, 0)
  self.textMesh.transform = MatrixTranslation(textPos)
  self.shadowMesh.transform = MatrixTranslation(shadowPos)
end

function Label:text(s)
  if s~= nil then
    self._dirtyText = true
    self._text = s
    self:needsRedraw()
    self:needsLayout()
  else
    return self._text
  end
end

function Label:font(v)
  if v ~= nil then
    self._dirtyText = true
    self._font = v
    self:needsRedraw()
    self:needsLayout()
  else
    return self._font
  end
end

function Label:fontSize(v)
  if v ~= nil then
    self._dirtyText = true  
    self._fontSize = v
    self:needsRedraw()
    self:needsLayout()
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
    self:needsLayout()
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
    self:needsLayout()
  else
    return self.__halign
  end
end

function Label:valign(v)
  if v ~= nil then
    self._valign = v
    self:needsLayout()
  else
    return self._valign
  end
end

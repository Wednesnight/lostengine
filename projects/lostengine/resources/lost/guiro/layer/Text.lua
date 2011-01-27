module("lost.guiro.layer", package.seeall)

require("lost.common.ShaderFactory")
require("lost.guiro.layer.Layer")
require("lost.guiro.Bounds")
require("lost.guiro.Quad")

lost.common.Class "lost.guiro.layer.Text" "lost.guiro.layer.Layer" {}

local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local MatrixTranslation = lost.math.MatrixTranslation
local Color = lost.common.Color

-- valid params:
-- * all layer params
-- * font: must be a table with two entries in this order: a string containing a font name known to the fontmanager, a number containing the desired fontsize
-- * text: the string to render
function Text:constructor(args)
  lost.guiro.layer.Layer.constructor(self, args)
  local t = args or {}
  self.id = t.id or "text"

  self.buffer = lost.font.TextBuffer()

  self.mesh = lost.font.RenderedText.create()
  self.mesh.material.shader = lost.guiro.shaderFactory():texture()
  self.mesh.material:blendNormal()
  
  self.shadowMesh = lost.font.RenderedText.create()
  self.shadowMesh.material.shader = lost.guiro.shaderFactory():texture()
  self.shadowMesh.material:blendNormal()
  
  self.shadowDrawNode = lost.rg.Draw.create(self.shadowMesh)
  self.renderNode:add(self.shadowDrawNode)
  self.drawNode = lost.rg.Draw.create(self.mesh)
  self.renderNode:add(self.drawNode)
  
  if t.font then self:font(t.font) end
  self:text(t.text or "")
  self:color(t.color or Color(0,0,0))
  self:shadow(t.shadow or false)
  self:shadowOffset(t.shadowOffset or Vec2(1,1))
  self:shadowColor(t.shadowColor or Color(0,0,0,.3))
  self:halign(t.halign or "center")
  self:valign(t.valign or "center")

  self:needsDisplay()
end

function Text:updateAlign()
  local textPos = Vec3(self.rect.x, self.rect.y, 0)

  if self._halign == "center" then
    textPos.x = self.rect.x + (self.rect.width-self.mesh.size.width)/2
  elseif self._halign == "left" then
    textPos.x = self.rect.x
  elseif self._halign == "right" then
    textPos.x = self.rect.x+self.rect.width - self.mesh.size.width
  end

  if self._valign == "center" then
      textPos.y = self.rect.y+((self.rect.height-(self.mesh.numLines*math.floor(self._font.lineHeight)))/2)+math.abs(self._font.descender)
  elseif self._valign == "top" then
    textPos.y = self.rect.y+self.rect.height-self.mesh.max.y
  elseif self._valign == "bottom" then
    textPos.y = self.rect.y-self._font.descender+1
  end
  
  textPos.x = math.floor(textPos.x)
  textPos.y = math.floor(textPos.y)

  self.mesh.transform = MatrixTranslation(textPos)  
  self.shadowMesh.transform = MatrixTranslation(textPos+Vec3(self._shadowOffset.x, self._shadowOffset.y,0))
end

function Text:updateLayout()
  lost.guiro.layer.Layer.updateLayout(self)
  self:needsDisplay()
  self:updateAlign()
end

function Text:updateDisplay()
  lost.guiro.layer.Layer.updateDisplay(self)
  self.buffer:reset(self._text, self._font, 1,self.rect.width)
  if self._font then
--      lost.font.render(self._text, self._font, self.mesh)
      self.buffer:renderAllPhysicalLines(self.mesh)
    if self.shadowDrawNode.active then
--      lost.font.render(self._text, self._font, self.shadowMesh)
      self.shadowMesh = self.mesh:clone()
      self.shadowMesh.material = self.mesh.material:clone()
      self.shadowMesh.material.color = self._shadowColor
      self.shadowDrawNode.mesh = self.shadowMesh
--      self.buffer:renderPhysicalLine(0, self.shadowMesh)
    end
  else
    log.warn("called updateDisplay on Text layer '"..self.id.."' without font")
  end
  self:updateAlign()
end

function Text:text(s)
  self._text = s
  self:needsDisplay()
end

function Text:font(...)
  if arg.n >=1 then
    self._font = tasklet.fontManager:getFont(arg[1][1], arg[1][2])
    self:needsDisplay()
  else
    return self._font
  end
end

function Text:color(v)
  self.mesh.material.color = v
end

function Text:valign(v)
  self._valign = v
  self:needsLayout()
end

function Text:halign(v)
  self._halign = v
  self:needsLayout()
end

function Text:shadow(...)
  if arg.n >= 1 then
    self.shadowDrawNode.active = arg[1]
    self:needsDisplay()
  else
    return self.shadowDrawNode.active
  end
end

function Text:shadowOffset(...)
  if arg.n >= 1 then
    self._shadowOffset = arg[1]
    self:needsLayout()
  else
    return self._shadowOffset
  end
end

function Text:shadowColor(...)
  if arg.n >= 1 then
    self._shadowColor = arg[1]
    self.shadowMesh.material.color = self._shadowColor
  else
    return self.shadowMesh.color
  end
end



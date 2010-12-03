module("lost.guiro.layer", package.seeall)

require("lost.guiro.Singletons")
require("lost.guiro.Bounds")
require("lost.guiro.Quad")

lost.common.Class "lost.guiro.layer.Text" "lost.guiro.layer.Layer" {}

local Vec2 = lost.math.Vec2

-- valid params:
-- * all layer params
-- * font: must be a table with two entries in this order: a string containing a font name known to the fontmanager, a number containing the desired fontsize
-- * text: the string to render
-- * 
function Text:constructor(args)
  lost.guiro.layer.Layer.constructor(self, args)
  local t = args or {}
  if t.font ~= nil then -- font must contain a name and a size in this order
    self.font = tasklet.fontManager:getFont(t.font[1], t.font[2])
  else
    log.warn("Text layer '"..self.id.."' constructed without font!")
    self.font = nil
  end
  self.id = t.id or "text"
  self.text = t.text or ""
  self.mesh = lost.font.RenderedText.create()
  self.mesh.material.shader = lost.guiro.shaderFactory():texture()
  self.mesh.material.color = t.color or Color(1,1,1)
  self.mesh.material:blendNormal()
  self.drawNode = lost.rg.Draw.create(self.mesh)
  self.renderNode:add(self.drawNode)
  self._halign = t.halign or "center"
  self._valign = t.valign or "center"
  self:needsLayout()
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
      textPos.y = self.rect.y+((self.rect.height-self.mesh.fontHeight)/2)+math.abs(self.mesh.fontDescender)
  elseif self._valign == "top" then
    textPos.y = self.rect.y+self.rect.height-self.mesh.fontAscender
  elseif self._valign == "bottom" then
    textPos.y = self.rect.y-self.mesh.fontDescender+1
  end
  
  textPos.x = math.floor(textPos.x)
  textPos.y = math.floor(textPos.y)

  self.mesh.transform = MatrixTranslation(textPos)  
end

function Text:updateLayout()
  lost.guiro.layer.Layer.updateLayout(self)
  self:updateAlign()
end

function Text:updateDisplay()
  lost.guiro.layer.Layer.updateDisplay(self)
  if self.font then
    self.font:render(self.text, self.mesh)
  else
    log.warn("called updateDisplay on Text layer '"..self.id.."' without font")
  end
  self:updateAlign()
end

function Text:text(s)
  self.text = s
  self:needsDisplay()
end
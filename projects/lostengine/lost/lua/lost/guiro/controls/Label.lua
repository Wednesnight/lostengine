module("lost.guiro.controls", package.seeall)

require("lost.guiro.View")

--[[
     Label control
  ]]
Label = lost.common.Class("lost.guiro.controls.Label", lost.guiro.View)

--[[
    constructor
  ]]
function Label:__init(properties)
  properties = properties or {}

  -- initialize private members
  self.textChanged = false

  -- initialize defaults
  properties.align = properties.align or "center"
  properties.fontColor = properties.fontColor or lost.common.Color(1,1,1)
  properties.font = properties.font or lost.guiro.config.defaultFont
  properties.fontSize = properties.fontSize or 12

  lost.guiro.View.__init(self, properties)
end

function Label:setProperty(key, value)
  if key == "text" then
    self:setText(value)
    return true
  end
  return false
end

function Label:setText(text)
	self.text = text
	self.textChanged = true
	self:needsRedraw()
end

function Label:redraw(canvas)
  lost.guiro.View.redraw(self, canvas)

  -- update text
  if self.textChanged then
  	self.renderedText = self.font:render(self.text, self.fontSize)
  	self.textChanged = false
  end

  if self.renderedText then
    -- get the label rect
  	local gr = self:globalRect()

    if self.renderedText.size.width > gr.width then
      local textLen = math.modf(gr.width / (self.renderedText.size.width / string.len(self.text))) - 3
      local text = string.sub(self.text, 1, textLen) .."..."
  		self.renderedText = self.font:render(text, self.fontSize)		
    end

    -- text should be vertically centered
  	gr.y = math.modf(gr.y + ((gr.height - self.renderedText.size.height - self.renderedText.min.y + 1) / 2))

    -- alignment
    if self.align == "center" then
      gr.x = math.modf(gr.x + ((gr.width - self.renderedText.size.width - self.renderedText.min.x - 1) / 2))
    elseif self.align == "right" then
      gr.x = gr.x + (gr.width - self.renderedText.size.width - self.renderedText.min.x - 1)
    end

    -- draw text
  	canvas:setColor(self.fontColor)
  	gl.glPushMatrix()
  	gl.glTranslate(gr.x, gr.y, 0)
  	self.renderedText:render(canvas)
  	gl.glPopMatrix()
  end
end

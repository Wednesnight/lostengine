module("lost.guiro.themes.default.renderers", package.seeall)

require("lost.guiro.Renderer")

--[[
     Label Renderer
  ]]
Label = lost.common.Class("lost.guiro.themes.default.renderers.Label", lost.guiro.Renderer)

function Label:__init(properties) lost.guiro.Renderer.__init(self, properties)
  properties = properties or {}
end

function Label:render(canvas, label, style) lost.guiro.Renderer.render(self, canvas, label, style)

  -- update text
	if label.textChanged then
		self.renderedText = style.font:render(label.text, style.fontSize)		
		label.textChanged = false
	end

  -- get the label rect
	local gr = label:globalRect()

  if self.renderedText.size.width > gr.width then
    local textLen = math.modf(gr.width / (self.renderedText.size.width / string.len(label.text))) - 3
    local text = string.sub(label.text, 1, textLen) .."..."
		self.renderedText = style.font:render(text, style.fontSize)		
  end

  -- text should be vertically centered
	gr.y = math.modf(gr.y + ((gr.height - self.renderedText.size.height - self.renderedText.min.y + 1) / 2))

  -- alignment
  if style.align == "center" then
    gr.x = math.modf(gr.x + ((gr.width - self.renderedText.size.width - self.renderedText.min.x - 1) / 2))
  elseif style.align == "right" then
    gr.x = gr.x + (gr.width - self.renderedText.size.width - self.renderedText.min.x - 1)
  end

  -- draw text
	canvas:setColor(style.color)
	gl.glPushMatrix()
	gl.glTranslate(gr.x, gr.y, 0)
	self.renderedText:render(canvas)
	gl.glPopMatrix()
end

module("lost.guiro.controls", package.seeall)

require("lost.guiro.View")

--[[
     Label control
  ]]
lost.common.Class "lost.guiro.controls.Label" "lost.guiro.View" {}

--[[
    constructor
  ]]
function Label:create(properties)
  properties = properties or {}

  -- initialize private members
  self.textChanged = false

  -- initialize defaults
  properties.style = properties.style or {}
  properties.style.align = properties.style.align or "center"
  properties.style.fontColor = properties.style.fontColor or lost.common.Color(1,1,1)
  properties.style.font = properties.style.font or lost.guiro.config.defaultFont
  properties.style.fontSize = properties.style.fontSize or 12

  lost.guiro.View.create(self, properties)
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

function Label:update()
  -- update text
  if self.textChanged then
	  self.textChanged = false
  	self.meshes["text"] = self.style.font:render(self.text, self.style.fontSize)

--[[ FIXME: we have to push real types, not bases. until then we're not able to position stuff
    -- get the label rect
  	local gr = self:globalRect()

    if self.meshes["text"].size.width > gr.width then
      local textLen = math.modf(gr.width / (self.meshes["text"].size.width / string.len(self.text))) - 3
      local text = string.sub(self.text, 1, textLen) .."..."
  		self.meshes["text"] = self.style.font:render(text, self.style.fontSize)
    end

    -- text should be vertically centered
  	gr.y = math.modf(gr.y + ((gr.height - self.meshes["text"].size.height - self.meshes["text"].min.y + 1) / 2))

    -- alignment
    if self.style.align == "center" then
      gr.x = math.modf(gr.x + ((gr.width - self.meshes["text"].size.width - self.meshes["text"].min.x - 1) / 2))
    elseif self.style.align == "right" then
      gr.x = gr.x + (gr.width - self.meshes["text"].size.width - self.meshes["text"].min.x - 1)
    end
]]
    -- update nodes
    if self.textNode then
      self.renderNode:remove(self.textNode)
    end
    self.textNode = lost.rg.Draw.create(self.meshes["text"])
    self.renderNode:add(self.textNode)

    self:needsLayout()
    self:needsRedraw()
  end
  lost.guiro.View.update(self)
end

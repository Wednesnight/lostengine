module("lost.guiro", package.seeall)

require("lost.guiro.View")
require("lost.guiro.Bounds")

local Rect = lost.math.Rect

lost.common.Class "lost.guiro.Label" "lost.guiro.View" {}

-- render triggers
-- * text
-- * fontSize
-- * font

-- layout triggers
-- * layout
-- * shadowOffset

--[[  A Label displays one line of text inside it's bounds.
  Text is rendered with the given font and fontsize.
  Optionally, a shadow can be drawn underneath the original text with a flexible offset
  and different color.
]]
function Label:constructor()
  lost.guiro.View.constructor(self)

  self.font = nil
  self.fontSize = 12
  self.textMesh = lost.font.RenderedText.create()
  self.shadowMesh = lost.font.RenderedText.create()
  self.textNode = lost.rg.Draw.create(self.normalMesh)
  self.shadowNode = lost.rg.Draw.create(self.shadowMesh)
  self.renderNode:add(self.shadowNode)
  self.renderNode:add(self.textNode)

  -- textData is interpreted as UTF-8 character data
  self.textData = nil -- label is empty by default since we can't load a default font from here and prerender something
  -- textColor is managed inside textMesh.material.color
  self.shadowOffset = Vec2(1,-1)
  -- shadowColor is managed inside shadowMesh.material.color
  -- showShadow is managed as shadowNode.active
  self.halign = "center" -- possible values: left, center, right
  self.valign = "center" -- possible values: top, center, bottom
  
  -- suppress background and frame by default
  self:showFrame(false)
  self:showBackground(false)
end

function Label:layout()
  
end

-- call this after a param change to update the appearance of text and shadow Mesh
-- will recreate the mesh data (but not the meshes)
function Label:render()
  self.font:render(self.textData, self.fontSize, self.textMesh)
  self.font:render(self.textData, self.fontSize, self.shadowMesh)
end

function Label:text(s)
  if s~= nil then
      self.textData = s
    else
      log.error("can't update label "..id.." because font is nil.")
    end
  else
    return self.textData
  end
end
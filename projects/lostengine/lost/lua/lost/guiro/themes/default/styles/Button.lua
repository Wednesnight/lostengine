module("lost.guiro.themes.default.styles", package.seeall)

require("lost.guiro.Style")

--[[
     Button Style
  ]]
Button = lost.common.Class("lost.guiro.themes.default.styles.Button", lost.guiro.Style)

function Button:__init(properties) lost.guiro.Style.__init(self, properties)
  properties = properties or {}

  properties.released = properties.released or {}
  properties.released.backgroundColor = properties.released.backgroundColor or lost.common.Color(0.5,0.5,0.5,1)
  properties.released.fontColor = properties.released.fontColor or lost.common.Color(1,1,1,1)
  self.released = lost.guiro.Style(properties.released)

  properties.hovered = properties.hovered or {}
  properties.hovered.backgroundColor = properties.hovered.backgroundColor or lost.common.Color(0.6,0.6,0.6,1)
  properties.hovered.fontColor = properties.hovered.fontColor or lost.common.Color(1,1,1,1)
  self.hovered = lost.guiro.Style(properties.hovered)

  properties.pressed = properties.pressed or {}
  properties.pressed.backgroundColor = properties.pressed.backgroundColor or lost.common.Color(0.3,0.3,0.3,1)
  properties.pressed.fontColor = properties.pressed.fontColor or lost.common.Color(1,1,1,1)
  self.pressed = lost.guiro.Style(properties.pressed)
end

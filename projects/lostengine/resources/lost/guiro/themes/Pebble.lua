module("lost.guiro.themes", package.seeall) 

require("lost.common.Class")
require("lost.guiro.Theme")

require("lost.guiro.layer.Text")

lost.common.Class "lost.guiro.themes.Pebble" "lost.guiro.Theme" {}

local Color = lost.common.Color

function Pebble:constructor()
  lost.guiro.Theme.constructor(self)
  self.name = "pebble"
  self:addStyle("lost.guiro.view.Label", "default", function(target, args) self:labelDefault(target, args) end)
end

function Pebble:labelDefault(target, args)
  target.layer:addSublayer(lost.guiro.layer.Text
                            {
                              bounds={0,0,"1","1"},
                              color = Color(0,0,0),
                              font = {"Vera", 12}
                            })
end


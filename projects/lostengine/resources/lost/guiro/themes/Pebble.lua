module("lost.guiro.themes", package.seeall) 

require("lost.common.Class")
require("lost.guiro.Theme")

require("lost.guiro.layer.Text")
require("lost.guiro.layer.RoundedRect")

lost.common.Class "lost.guiro.themes.Pebble" "lost.guiro.Theme" {}

local Color = lost.common.Color

function Pebble:constructor()
  lost.guiro.Theme.constructor(self)
  self.name = "pebble"
  self:addStyle("lost.guiro.view.Label", "default", function(target, args) self:labelDefault(target, args) end)
  self:addStyle("lost.guiro.view.Label", "round", function(target, args) self:labelRound(target, args) end)
  self:addStyle("lost.guiro.view.Label", "roundFramed", function(target, args) self:labelRoundFramed(target, args) end)
  self:addStyle("lost.guiro.view.Button", "default", function(target, args) self:buttonGray(target, args) end)
  self:addStyle("lost.guiro.view.View", "gray", function(target, args) self:viewGray(target, args) end)
--  self:addStyle("lost.guiro.view.View", "default", function(target, args) self:viewGray(target, args) end)
end

function Pebble:labelDefault(target, args)
  target.layer:addSublayer(lost.guiro.layer.Text
                            {
                              bounds={0,0,"1","1"},
                              color = Color(0,0,0),
                              font = {"Vera", 12}
                            })
end

function Pebble:labelRound(target, args)
  target.layer:addSublayer(lost.guiro.layer.RoundedRect
                            {
                              bounds={0,0,"1","1"},
                              color = Color(1,1,1),
                              radius=8,
                              filled = true
                            })
  target.layer:addSublayer(lost.guiro.layer.Text
                            {
                              bounds={0,0,"1","1"},
                              color = Color(0,0,0),
                              font = {"Vera", 12}
                            })
end

function Pebble:labelRoundFramed(target, args)
  target.layer:addSublayer(lost.guiro.layer.RoundedRect
                            {
                              bounds={0,0,"1","1"},
                              color = Color(1,1,1),
                              radius=8,
                              filled = true
                            })
  target.layer:addSublayer(lost.guiro.layer.RoundedRect
                            {
                              bounds={0,0,"1","1"},
                              color = Color(0,0,0),
                              radius=8,
                              filled = false,
                              width=1
                            })
  target.layer:addSublayer(lost.guiro.layer.Text
                            {
                              bounds={0,0,"1","1"},
                              color = Color(0,0,0),
                              font = {"Vera", 12}
                            })
end


function Pebble:buttonGray(target,args)
  local b = lost.guiro.view.Button
  local normal = lost.guiro.layer.RoundedRect{
    bounds = {0,0,"1","1"},
    color = Color(1,0,0),
    filled = true,
    radius = 8
  }
--[[  local hover = lost.guiro.layer.RoundedRect{
    bounds = {0,0,"1","1"},
    color = Color(1,.5,0),
    filled = true,
    radius = 8
  }]]
  local pushed = lost.guiro.layer.RoundedRect{
    bounds = {0,0,"1","1"},
    color = Color(1,1,0),
    filled = true,
    radius = 8
  }
  local disabled = lost.guiro.layer.RoundedRect{
    bounds = {0,0,"1","1"},
    color = Color(.3,.3,.3),
    filled = true,
    radius = 8
  }
  target.layer:addSublayer(normal)
--  target.layer:addSublayer(hover)
  target.layer:addSublayer(pushed)
  target.layer:addSublayer(disabled)
  target.backgrounds[b.STATE_NORMAL] = normal
--  target.backgrounds[b.STATE_HOVER] = hover
  target.backgrounds[b.STATE_PUSHED] = pushed
  target.backgrounds[b.STATE_DISABLED] = disabled
end

function Pebble:viewGray(target, args)
  target.layer:addSublayer(lost.guiro.layer.Rect
                            {
                              bounds={0,0,"1","1"},
                              color = Color(.9294,.9294,.9294),
                              filled = true
                            })  
end

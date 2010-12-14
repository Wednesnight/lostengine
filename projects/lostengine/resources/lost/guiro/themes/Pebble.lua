module("lost.guiro.themes", package.seeall) 

require("lost.common.Class")
require("lost.guiro.Theme")

require("lost.guiro.layer.Text")
require("lost.guiro.layer.HLine")
require("lost.guiro.layer.RoundedRect")
require("lost.guiro.themes.PebbleGradients")

lost.common.Class "lost.guiro.themes.Pebble" "lost.guiro.Theme" {}

local Color = lost.common.Color


function Pebble:constructor()
  lost.guiro.Theme.constructor(self)
  pebbleBuildGradients(lost.guiro.textureManager())
  self.name = "pebble"
  self:addStyle("lost.guiro.view.Label", "default", function(target, args) self:labelDefault(target, args) end)
  self:addStyle("lost.guiro.view.Label", "round", function(target, args) self:labelRound(target, args) end)
  self:addStyle("lost.guiro.view.Label", "roundFramed", function(target, args) self:labelRoundFramed(target, args) end)
  self:addStyle("lost.guiro.view.Button", "default", function(target, args) self:buttonRounded(target, args) end)
  self:addStyle("lost.guiro.view.Button", "rounded", function(target, args) self:buttonRounded(target, args) end)
  self:addStyle("lost.guiro.view.Button", "roundedToggle", function(target, args) self:buttonRoundedToggle(target, args) end)
  self:addStyle("lost.guiro.view.View", "default", function(target, args)  end)
  self:addStyle("lost.guiro.view.View", "gray", function(target, args) self:viewGray(target, args) end)
  self:addStyle("lost.guiro.view.View", "toolbar", function(target, args) self:viewToolbar(target, args) end)
  self:addStyle("lost.guiro.view.UserInterface", "default", function(target, args) self:viewGray(target, args) end)

  self:addStyle("lost.guiro.view.Button", "tabCandyRoundSingle", function(target, args) self:buttonTabCandyRoundSingle(target, args) end)
  self:addStyle("lost.guiro.view.Button", "tabCandyRoundLeft", function(target, args) self:buttonTabCandyRoundLeft(target, args) end)
  self:addStyle("lost.guiro.view.Button", "tabCandyRoundMid", function(target, args) self:buttonTabCandyRoundMid(target, args) end)
  self:addStyle("lost.guiro.view.Button", "tabCandyRoundRight", function(target, args) self:buttonTabCandyRoundRight(target, args) end)

  self:addStyle("lost.guiro.view.TabBar", "default", function(target, args) self:tabBarCandy(target, args) end)
  self:addStyle("lost.guiro.view.TabBar", "candy", function(target, args) self:tabBarCandy(target, args) end)

  self.buttonRoundedHeight = {mini=14, small=16, regular=18}
  self.buttonRoundedFonts = {mini={"Vera", 9}, small={"Vera", 10}, regular={"Vera", 11}}
  self.buttonRoundedFrameCol = Color(.6588,.6588,.6588)
--  self:addStyle("lost.guiro.view.View", "default", function(target, args) self:viewGray(target, args) end)
end

function Pebble:labelDefault(target, args)
  target.layer:addSublayer(lost.guiro.layer.Text{bounds={0,0,"1","1"},color=Color(0,0,0),font = {"Vera", 12}})
end

function Pebble:labelRound(target, args)
  target.layer:addSublayer(lost.guiro.layer.RoundedRect{bounds={0,0,"1","1"},color=Color(1,1,1),radius=8,filled=true})
  target.layer:addSublayer(lost.guiro.layer.Text{bounds={0,0,"1","1"},color = Color(0,0,0),font = {"Vera", 12}})
end

function Pebble:labelRoundFramed(target, args)
  target.layer:addSublayer(lost.guiro.layer.RoundedRect{bounds={0,0,"1","1"},color=Color(1,1,1),radius=8,filled=true})
  target.layer:addSublayer(lost.guiro.layer.RoundedRect{bounds={0,0,"1","1"},color=Color(0,0,0),radius=8,filled = false,width=1})
  target.layer:addSublayer(lost.guiro.layer.Text{bounds={0,0,"1","1"},color=Color(0,0,0),font={"Vera", 12}})
end

function Pebble:buttonRounded(target,args)
  local l = lost.guiro.layer.Layer
  local rr = lost.guiro.layer.RoundedRect

  local b = lost.guiro.view.Button
  local size = args.size or "small"
  -- target bounds must have been set by now so we can modify height
  target._bounds.height = lost.guiro.Bounds.decodeEntry(4,self.buttonRoundedHeight[size])
  local r = self.buttonRoundedHeight[size]/2 -- rounded rect radius
  local normal = l{sublayers={rr{bounds={0,0,"1","1"},gradient="rrbg",filled=true,radius=r},
                              rr{bounds={0,0,"1","1"},color=self.buttonRoundedFrameCol,filled=false,radius=r}}
                             }
  local pushed = l{sublayers={rr{bounds={0,0,"1","1"},gradient="rrbg2",filled=true,radius=r},
                              rr{bounds={0,0,"1","1"},color=self.buttonRoundedFrameCol,filled=false,radius=r}}
                             }
  local text = lost.guiro.layer.Text{bounds={0,0,"1","1"},font=self.buttonRoundedFonts[size],color=Color(0,0,0)}
  target.layer:addSublayer(normal)
  target.layer:addSublayer(pushed)
  target.layer:addSublayer(text)
  target.textLayer = text
  target.backgrounds[b.STATE_NORMAL] = normal
  target.backgrounds[b.STATE_PUSHED] = pushed
  target.titleColors[b.STATE_NORMAL] = Color(0,0,0)
  target.titleColors[b.STATE_PUSHED] = Color(1,1,1)
end

function Pebble:buttonRoundedToggle(target,args)
  local l = lost.guiro.layer.Layer
  local rr = lost.guiro.layer.RoundedRect

  local b = lost.guiro.view.Button
  local size = args.size or "small"
  -- target bounds must have been set by now so we can modify height
  target._bounds.height = lost.guiro.Bounds.decodeEntry(4,self.buttonRoundedHeight[size])
  local r = self.buttonRoundedHeight[size]/2 -- rounded rect radius
  local normal = l{id="normal",sublayers={rr{bounds={0,0,"1","1"},gradient="candyGray",filled=true,radius=r},
                              rr{bounds={0,0,"1","1"},gradient="candyGrayFrame",filled=false,radius=r}}
                             }
--[[ local hover = l{id="hover",sublayers={rr{bounds={0,0,"1","1"},color=Color(1,1,0),filled=true,radius=r},
                             rr{bounds={0,0,"1","1"},color=self.buttonRoundedFrameCol,filled=false,radius=r}}
                            }]]
  local pushed = l{id="pushed",sublayers={rr{bounds={0,0,"1","1"},gradient="candyGray",color=Color(.9,.9,.9),filled=true,radius=r},
                              rr{bounds={0,0,"1","1"},gradient="candyGrayFrame",filled=false,radius=r}}
                             }

  local normal2 = l{id="normal2",sublayers={rr{bounds={0,0,"1","1"},gradient="candyBlue",filled=true,radius=r},
                               rr{bounds={0,0,"1","1"},gradient="candyBlueFrame",filled=false,radius=r}}
                              }
--[[  local hover2 = l{id="hover2",sublayers={rr{bounds={0,0,"1","1"},color=Color(1,1,.5),filled=true,radius=r},
                              rr{bounds={0,0,"1","1"},color=self.buttonRoundedFrameCol,filled=false,radius=r}}
                             }]]
  local pushed2 = l{id="pushed2",sublayers={rr{bounds={0,0,"1","1"},gradient="candyBlue",color=Color(.9,.9,.9),filled=true,radius=r},
                               rr{bounds={0,0,"1","1"},gradient="candyBlueFrame",filled=false,radius=r}}
                              }

  local text = lost.guiro.layer.Text{bounds={0,0,"1","1"},font=self.buttonRoundedFonts[size],color=Color(0,0,0)}
  target.layer:addSublayer(normal)
  target.layer:addSublayer(pushed)
--  target.layer:addSublayer(hover)
  target.layer:addSublayer(normal2)
  target.layer:addSublayer(pushed2)
--  target.layer:addSublayer(hover2)
  target.layer:addSublayer(text)
  target.textLayer = text
  target.backgrounds[b.STATE_NORMAL] = normal
  target.backgrounds[b.STATE_PUSHED] = pushed
--  target.backgrounds[b.STATE_HOVER] = hover
  target.backgrounds[b.STATE_NORMAL2] = normal2
  target.backgrounds[b.STATE_PUSHED2] = pushed2
  --  target.backgrounds[b.STATE_HOVER2] = hover2
  target.titleColors[b.STATE_NORMAL] = Color(0,0,0)
  target.titleColors[b.STATE_DISABLED] = Color(.8,.8,.8)
end

function Pebble:viewGray(target, args)
  target.layer:addSublayer(lost.guiro.layer.Rect{bounds={0,0,"1","1"},color = Color(.9294,.9294,.9294),filled = true})  
end

function Pebble:viewToolbar(target, args)
  target.layer:addSublayer(lost.guiro.layer.Rect{bounds={0,0,"1","1"},gradient="toolbarBg",filled = true})  
  target.layer:addSublayer(lost.guiro.layer.HLine{bounds={0,0,"1","1"},color=Color(.317,.317,.317)})  
end

-------------
-- tabbar button styles
--

-- round candy

function Pebble:buttonTabCandyRoundSingle(target, args)
  local l = lost.guiro.layer.Layer
  local rr = lost.guiro.layer.RoundedRect

  local b = lost.guiro.view.Button
  local size = args.size or "small"
  target:mode("toggleOnce")
  -- target bounds must have been set by now so we can modify height
  target._bounds.height = lost.guiro.Bounds.decodeEntry(4,self.buttonRoundedHeight[size])
  local r = self.buttonRoundedHeight[size]/2 -- rounded rect radius
  local normal = l{sublayers={rr{bounds={0,0,"1","1"},gradient="candyGray",filled=true,radius=r},
                              rr{bounds={0,0,"1","1"},gradient="candyGrayFrame",filled=false,radius=r}}
                             }
  local pushed = l{sublayers={rr{bounds={0,0,"1","1"},gradient="candyGray",color=Color(.9,.9,.9),filled=true,radius=r},
                              rr{bounds={0,0,"1","1"},gradient="candyGrayFrame",filled=false,radius=r}}
                             }

  local normal2 = l{sublayers={rr{bounds={0,0,"1","1"},gradient="candyBlue",filled=true,radius=r},
                               rr{bounds={0,0,"1","1"},gradient="candyBlueFrame",filled=false,radius=r}}
                              }

  local text = lost.guiro.layer.Text{bounds={0,0,"1","1"},font=self.buttonRoundedFonts[size],color=Color(0,0,0)}
  target.layer:addSublayer(normal)
  target.layer:addSublayer(pushed)
  target.layer:addSublayer(normal2)
  target.layer:addSublayer(text)
  target.textLayer = text
  target.backgrounds[b.STATE_NORMAL] = normal
  target.backgrounds[b.STATE_PUSHED] = pushed
  target.backgrounds[b.STATE_NORMAL2] = normal2
  target.titleColors[b.STATE_NORMAL] = Color(0,0,0)
  target.titleColors[b.STATE_DISABLED] = Color(.8,.8,.8)  
end

function Pebble:buttonTabCandyRoundLeft(target, args)
  local l = lost.guiro.layer.Layer
  local rr = lost.guiro.layer.RoundedRect

  local b = lost.guiro.view.Button
  local size = args.size or "small"
  target:mode("toggleOnce")
  -- target bounds must have been set by now so we can modify height
  target._bounds.height = lost.guiro.Bounds.decodeEntry(4,self.buttonRoundedHeight[size])
  local r = self.buttonRoundedHeight[size]/2 -- rounded rect radius
  local normal = l{sublayers={rr{bounds={0,0,"1","1"},gradient="candyGray",filled=true,radius=r, roundCorners={tl=true, bl=true, br=false, tr=false}},
                              rr{bounds={0,0,"1","1"},gradient="candyGrayFrame",filled=false,radius=r, roundCorners={tl=true, bl=true, br=false, tr=false}}}
                             }
  local pushed = l{sublayers={rr{bounds={0,0,"1","1"},gradient="candyGray",color=Color(.9,.9,.9),filled=true,radius=r,roundCorners={tl=true, bl=true, br=false, tr=false}},
                              rr{bounds={0,0,"1","1"},gradient="candyGrayFrame",filled=false,radius=r,roundCorners={tl=true, bl=true, br=false, tr=false}}}
                             }

  local normal2 = l{sublayers={rr{bounds={0,0,"1","1"},gradient="candyBlue",filled=true,radius=r,roundCorners={tl=true, bl=true, br=false, tr=false}},
                               rr{bounds={0,0,"1","1"},gradient="candyBlueFrame",filled=false,radius=r,roundCorners={tl=true, bl=true, br=false, tr=false}}}
                              }

  local text = lost.guiro.layer.Text{bounds={0,0,"1","1"},font=self.buttonRoundedFonts[size],color=Color(0,0,0)}
  target.layer:addSublayer(normal)
  target.layer:addSublayer(pushed)
  target.layer:addSublayer(normal2)
  target.layer:addSublayer(text)
  target.textLayer = text
  target.backgrounds[b.STATE_NORMAL] = normal
  target.backgrounds[b.STATE_PUSHED] = pushed
  target.backgrounds[b.STATE_NORMAL2] = normal2
  target.titleColors[b.STATE_NORMAL] = Color(0,0,0)
  target.titleColors[b.STATE_DISABLED] = Color(.8,.8,.8)  
end

function Pebble:buttonTabCandyRoundMid(target, args)
  local l = lost.guiro.layer.Layer
  local rr = lost.guiro.layer.RoundedRect
  local rc = lost.guiro.layer.Rect

  local b = lost.guiro.view.Button
  local size = args.size or "small"
  target:mode("toggleOnce")
  -- target bounds must have been set by now so we can modify height
  target._bounds.height = lost.guiro.Bounds.decodeEntry(4,self.buttonRoundedHeight[size])
  local r = self.buttonRoundedHeight[size]/2 -- rounded rect radius
  local normal = l{sublayers={rc{bounds={0,0,"1","1"},gradient="candyGray",filled=true,},
                              rr{bounds={0,0,"1","1"},gradient="candyGrayFrame",filled=false,radius=r, roundCorners={tl=false, bl=false, br=false, tr=false},sides={left=false}}}
                             }
  local pushed = l{sublayers={rc{bounds={0,0,"1","1"},gradient="candyGray",color=Color(.9,.9,.9),filled=true},
                              rr{bounds={0,0,"1","1"},gradient="candyGrayFrame",filled=false,radius=r,roundCorners={tl=false, bl=false, br=false, tr=false},sides={left=false}}}
                             }

  local normal2 = l{sublayers={rc{bounds={0,0,"1","1"},gradient="candyBlue",filled=true,},
                               rr{bounds={0,0,"1","1"},gradient="candyBlueFrame",filled=false,radius=r,roundCorners={tl=false, bl=false, br=false, tr=false},sides={left=false}}}
                              }

  local text = lost.guiro.layer.Text{bounds={0,0,"1","1"},font=self.buttonRoundedFonts[size],color=Color(0,0,0)}
  target.layer:addSublayer(normal)
  target.layer:addSublayer(pushed)
  target.layer:addSublayer(normal2)
  target.layer:addSublayer(text)
  target.textLayer = text
  target.backgrounds[b.STATE_NORMAL] = normal
  target.backgrounds[b.STATE_PUSHED] = pushed
  target.backgrounds[b.STATE_NORMAL2] = normal2
  target.titleColors[b.STATE_NORMAL] = Color(0,0,0)
  target.titleColors[b.STATE_DISABLED] = Color(.8,.8,.8)  
end

function Pebble:buttonTabCandyRoundRight(target, args)
  local l = lost.guiro.layer.Layer
  local rr = lost.guiro.layer.RoundedRect

  local b = lost.guiro.view.Button
  local size = args.size or "small"
  target:mode("toggleOnce")
  -- target bounds must have been set by now so we can modify height
  target._bounds.height = lost.guiro.Bounds.decodeEntry(4,self.buttonRoundedHeight[size])
  local r = self.buttonRoundedHeight[size]/2 -- rounded rect radius
  local normal = l{sublayers={rr{bounds={0,0,"1","1"},gradient="candyGray",filled=true,radius=r, roundCorners={tl=false, bl=false, br=true, tr=true}},
                              rr{bounds={0,0,"1","1"},gradient="candyGrayFrame",filled=false,radius=r, roundCorners={tl=false, bl=false, br=true, tr=true},sides={left=false}}}
                             }
  local pushed = l{sublayers={rr{bounds={0,0,"1","1"},gradient="candyGray",color=Color(.9,.9,.9),filled=true,radius=r,roundCorners={tl=false, bl=false, br=true, tr=true},sides={left=false}},
                              rr{bounds={0,0,"1","1"},gradient="candyGrayFrame",filled=false,radius=r,roundCorners={tl=false, bl=false, br=true, tr=true},sides={left=false}}}
                             }

  local normal2 = l{sublayers={rr{bounds={0,0,"1","1"},gradient="candyBlue",filled=true,radius=r,roundCorners={tl=false, bl=false, br=true, tr=true},sides={left=false}},
                               rr{bounds={0,0,"1","1"},gradient="candyBlueFrame",filled=false,radius=r,roundCorners={tl=false, bl=false, br=true, tr=true},sides={left=false}}}
                              }

  local text = lost.guiro.layer.Text{bounds={0,0,"1","1"},font=self.buttonRoundedFonts[size],color=Color(0,0,0)}
  target.layer:addSublayer(normal)
  target.layer:addSublayer(pushed)
  target.layer:addSublayer(normal2)
  target.layer:addSublayer(text)
  target.textLayer = text
  target.backgrounds[b.STATE_NORMAL] = normal
  target.backgrounds[b.STATE_PUSHED] = pushed
  target.backgrounds[b.STATE_NORMAL2] = normal2
  target.titleColors[b.STATE_NORMAL] = Color(0,0,0)
  target.titleColors[b.STATE_DISABLED] = Color(.8,.8,.8)  
end

------------
-- tabbar styles

function Pebble:tabBarCandy(target, args)
  local size = args.size or "regular"

  target.singleButtonStyleParams.size = size
  target.singleButtonStyleParams.style = "tabCandyRoundSingle"

  target.leftButtonStyleParams.size = size 
  target.leftButtonStyleParams.style = "tabCandyRoundLeft" 

  target.midButtonStyleParams.size = size
  target.midButtonStyleParams.style = "tabCandyRoundMid"

  target.rightButtonStyleParams.size = size
  target.rightButtonStyleParams.style = "tabCandyRoundRight"
  
  target.buttonSizeAdjust = 30
end
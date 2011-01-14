module("lost.guiro.themes", package.seeall) 

require("lost.common.Class")
require("lost.guiro.Theme")

require("lost.guiro.layer.Text")
require("lost.guiro.layer.HLine")
require("lost.guiro.layer.RoundedRect")
require("lost.guiro.layer.Image")
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

  self:addStyle("lost.guiro.view.Button", "tabSquareSingle", function(target, args) self:buttonTabSquareSingle(target, args) end)
  self:addStyle("lost.guiro.view.Button", "tabSquareLeft", function(target, args) self:buttonTabSquareLeft(target, args) end)
  self:addStyle("lost.guiro.view.Button", "tabSquareMid", function(target, args) self:buttonTabSquareMid(target, args) end)
  self:addStyle("lost.guiro.view.Button", "tabSquareRight", function(target, args) self:buttonTabSquareRight(target, args) end)

  self:addStyle("lost.guiro.view.Button", "checkboxCandy", function(target, args) self:buttonCheckboxCandy(target, args) end)
  self:addStyle("lost.guiro.view.Button", "radioCandy", function(target, args) self:buttonRadioCandy(target, args) end)

  self:addStyle("lost.guiro.view.TabBar", "default", function(target, args) self:tabBarCandy(target, args) end)
  self:addStyle("lost.guiro.view.TabBar", "candy", function(target, args) self:tabBarCandy(target, args) end)
  self:addStyle("lost.guiro.view.TabBar", "square", function(target, args) self:tabBarSquare(target, args) end)

  self:addStyle("lost.guiro.view.RadioGroup", "default", function(target, args) end)

  self:addStyle("lost.guiro.view.View", "windowHeader", function(target, args) self:viewWindowHeader(target, args) end)
  self:addStyle("lost.guiro.view.View", "panelHeader", function(target, args) self:viewPanelHeader(target, args) end)
  self:addStyle("lost.guiro.view.View", "windowBack", function(target, args) self:viewWindowBack(target, args) end)
  self:addStyle("lost.guiro.view.Button", "windowCloseButton", function(target, args) self:buttonWindowClose(target, args) end)
  self:addStyle("lost.guiro.view.Window", "default", function(target, args) self:windowNormal(target, args) end)
  self:addStyle("lost.guiro.view.Window", "normal", function(target, args) self:windowNormal(target, args) end)
  self:addStyle("lost.guiro.view.Window", "panel", function(target, args) self:windowPanel(target, args) end)

  self:addStyle("lost.guiro.view.Box", "default", function(target, args) self:box(target, args) end)

  self:addStyle("lost.guiro.view.TabView", "default", function(target, args) self:tabviewCandyRoundedRecess(target, args) end)
  self:addStyle("lost.guiro.view.TabView", "candyRoundedRecess", function(target, args) self:tabviewCandyRoundedRecess(target, args) end)
  self:addStyle("lost.guiro.view.TabView", "candyRecess", function(target, args) self:tabviewCandyRecess(target, args) end)
  self:addStyle("lost.guiro.view.TabView", "square", function(target, args) self:tabviewSquare(target, args) end)

  self:addStyle("lost.guiro.view.Slider", "default", function(target, args) self:sliderCandy(target, args) end)
  self:addStyle("lost.guiro.view.Slider", "candy", function(target, args) self:sliderCandy(target, args) end)


  self.buttonRoundedHeight = {mini=14, small=16, regular=18}
  self.buttonRoundedFonts = {mini={"Vera", 9}, small={"Vera", 10}, regular={"Vera", 11}}
  self.buttonRoundedFrameCol = Color(.6588,.6588,.6588)
  
  self.checkboxRadius = 3
  self.checkboxSizes = {mini=10, small=12, regular=14}
  self.checkboxFonts = {mini={"Vera", 9}, small={"Vera", 10}, regular={"Vera", 11}}
  self.checkboxSpacing = 4
  
  self.radioSize = {regular=14,small=12,mini=10}
  self.radioFonts = {mini={"Vera", 9}, small={"Vera", 10}, regular={"Vera", 11}}
  self.radioCenterRadius = {regular=5,small=4,mini=3}
  self.radioSpacing = 4
  
  self.separatorColor = Color(.317,.317,.317)
  self.windowNormalHeaderHeight = 22
  self.windowPanelHeaderHeight = 16
  self.windowNormalCloseButtonSize = 14
  self.panelCloseButtonSize = 11
  self.windowCloseButtonSpacing = 8 --- distance to left window border
  self.panelCloseButtonSpacing = 6
  
  self.squareButtonLightFrameCol = Color(168/255, 168/255, 168/255)
  self.squareButtonDarkFrameCol = Color(145/255, 145/255, 145/255)
  
  self.sliderCandyTrackSize={regular=6, small=4,mini=2}
  self.sliderCandyHandleSize={regular=16, small=12,mini=10}
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

function Pebble:buttonTabSquareSingle(target, args)
  local l = lost.guiro.layer.Layer
  local r = lost.guiro.layer.Rect

  local b = lost.guiro.view.Button
  local size = args.size or "small"
  target:mode("toggleOnce")
  -- target bounds must have been set by now so we can modify height
  target._bounds.height = lost.guiro.Bounds.decodeEntry(4,self.buttonRoundedHeight[size])
  local normal = l{sublayers={r{bounds={0,0,"1","1"},gradient="squareGlass",filled=true},
                              r{bounds={0,0,"1","1"},color=self.squareButtonLightFrameCol,filled=false}}
                             }
  local pushed = l{sublayers={r{bounds={0,0,"1","1"},gradient="squareGlass",color=Color(.8,.8,.8),filled=true},
                              r{bounds={0,0,"1","1"},color=self.squareButtonDarkFrameCol,filled=false}}
                             }

  local normal2 = l{sublayers={r{bounds={0,0,"1","1"},gradient="squareGlass",color=Color(.9,.9,.9),filled=true},
                               r{bounds={0,0,"1","1"},color=self.squareButtonDarkFrameCol,filled=false}}
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

function Pebble:buttonTabSquareLeft(target, args)
  local l = lost.guiro.layer.Layer
  local rr = lost.guiro.layer.RoundedRect

  local b = lost.guiro.view.Button
  local size = args.size or "small"
  target:mode("toggleOnce")
  -- target bounds must have been set by now so we can modify height
  target._bounds.height = lost.guiro.Bounds.decodeEntry(4,self.buttonRoundedHeight[size])
  local normal = l{sublayers={rr{bounds={0,0,"1","1"},gradient="squareGlass",filled=true,roundCorners={tl=false, bl=false, br=false, tr=false}},
                              rr{bounds={0,0,"1","1"},color=self.squareButtonLightFrameCol,filled=false,roundCorners={tl=false, bl=false, br=false, tr=false}}}
                             }
  local pushed = l{sublayers={rr{bounds={0,0,"1","1"},gradient="squareGlass",color=Color(.8,.8,.8),filled=true,roundCorners={tl=false, bl=false, br=false, tr=false}},
                              rr{bounds={0,0,"1","1"},color=self.squareButtonDarkFrameCol,filled=false,roundCorners={tl=false, bl=false, br=false, tr=false}}}
                             }

  local normal2 = l{sublayers={rr{bounds={0,0,"1","1"},gradient="squareGlass",color=Color(.9,.9,.9),filled=true,roundCorners={tl=false, bl=false, br=false, tr=false}},
                               rr{bounds={0,0,"1","1"},color=self.squareButtonDarkFrameCol,filled=false,roundCorners={tl=false, bl=false, br=false, tr=false}}}
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

function Pebble:buttonTabSquareMid(target, args)
  local l = lost.guiro.layer.Layer
  local rr = lost.guiro.layer.RoundedRect
  local rc = lost.guiro.layer.Rect

  local b = lost.guiro.view.Button
  local size = args.size or "small"
  target:mode("toggleOnce")
  -- target bounds must have been set by now so we can modify height
  target._bounds.height = lost.guiro.Bounds.decodeEntry(4,self.buttonRoundedHeight[size])
  local normal = l{sublayers={rc{bounds={0,0,"1","1"},gradient="squareGlass",filled=true,},
                              rr{bounds={0,0,"1","1"},color=self.squareButtonLightFrameCol,filled=false,roundCorners={tl=false, bl=false, br=false, tr=false},sides={left=false}}}
                             }
  local pushed = l{sublayers={rc{bounds={0,0,"1","1"},gradient="squareGlass",color=Color(.8,.8,.8),filled=true},
                              rr{bounds={0,0,"1","1"},color=self.squareButtonDarkFrameCol,filled=false,roundCorners={tl=false, bl=false, br=false, tr=false},sides={left=false}}}
                             }

  local normal2 = l{sublayers={rc{bounds={0,0,"1","1"},gradient="squareGlass",color=Color(.9,.9,.9),filled=true,},
                               rr{bounds={0,0,"1","1"},color=self.squareButtonDarkFrameCol,filled=false,roundCorners={tl=false, bl=false, br=false, tr=false},sides={left=false}}}
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

function Pebble:buttonTabSquareRight(target, args)
  local l = lost.guiro.layer.Layer
  local rr = lost.guiro.layer.RoundedRect

  local b = lost.guiro.view.Button
  local size = args.size or "small"
  target:mode("toggleOnce")
  -- target bounds must have been set by now so we can modify height
  target._bounds.height = lost.guiro.Bounds.decodeEntry(4,self.buttonRoundedHeight[size])
  local normal = l{sublayers={rr{bounds={0,0,"1","1"},gradient="squareGlass",filled=true,roundCorners={tl=false, bl=false, br=false, tr=false}},
                              rr{bounds={0,0,"1","1"},color=self.squareButtonLightFrameCol,filled=false,roundCorners={tl=false, bl=false, br=false, tr=false},sides={left=false}}}
                             }
  local pushed = l{sublayers={rr{bounds={0,0,"1","1"},gradient="squareGlass",color=Color(.8,.8,.8),filled=true,roundCorners={tl=false, bl=false, br=false, tr=false},sides={left=false}},
                              rr{bounds={0,0,"1","1"},color=self.squareButtonDarkFrameCol,filled=false,roundCorners={tl=false, bl=false, br=false, tr=false},sides={left=false}}}
                             }

  local normal2 = l{sublayers={rr{bounds={0,0,"1","1"},gradient="squareGlass",color=Color(.9,.9,.9),filled=true,roundCorners={tl=false, bl=false, br=false, tr=false},sides={left=false}},
                               rr{bounds={0,0,"1","1"},color=self.squareButtonDarkFrameCol,filled=false,roundCorners={tl=false, bl=false, br=false, tr=false},sides={left=false}}}
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

function Pebble:tabBarSquare(target, args)
  local size = args.size or "regular"

  target.singleButtonStyleParams.size = size
  target.singleButtonStyleParams.style = "tabSquareSingle"

  target.leftButtonStyleParams.size = size 
  target.leftButtonStyleParams.style = "tabSquareLeft" 

  target.midButtonStyleParams.size = size
  target.midButtonStyleParams.style = "tabSquareMid"

  target.rightButtonStyleParams.size = size
  target.rightButtonStyleParams.style = "tabSquareRight"
  
  target.buttonSizeAdjust = 30
end


function Pebble:buttonRadioCandy(target, args)
  local l = lost.guiro.layer.Layer
  local d = lost.guiro.layer.Disc

  local b = lost.guiro.view.Button
  local size = args.size or "small"
  target:mode("toggleOnce")
  -- target bounds must have been set by now so we can modify height
  target._bounds.height = lost.guiro.Bounds.decodeEntry(4,self.radioSize[size])
  local r = self.radioSize[size]/2 
  local cr = self.radioCenterRadius[size]
  local normal = l{sublayers={d{bounds={0,0,"1","1"},gradient="candyGray",filled=true},
                              d{bounds={0,0,"1","1"},gradient="candyGrayFrame",filled=false}}
                             }
  local pushed = l{sublayers={d{bounds={0,0,"1","1"},gradient="candyGray",color=Color(.9,.9,.9),filled=true,},
                              d{bounds={0,0,"1","1"},gradient="candyGrayFrame",filled=false,}}
                             }

  local normal2 = l{sublayers={d{bounds={0,0,"1","1"},gradient="candyBlue",filled=true,},
                               d{bounds={0,0,"1","1"},gradient="candyBlueFrame",filled=false},
                               d{bounds={r-cr/2,r-cr/2,cr,cr},color=Color(0,0,0),filled=true}}
                              }
  local d = self.radioSize[size] + self.radioSpacing
  local text = lost.guiro.layer.Text{bounds={d,0,{"1",-d},"1"},font=self.radioFonts[size],color=Color(0,0,0),halign="left",valign="center"}
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

function Pebble:checkmarkTexture()
  if not self._checkmarkTexture then
    local data = tasklet.loader:load("lost/guiro/themes/checkmark.png")
    local bmp = lost.bitmap.Bitmap.create(data)
    bmp:premultiplyAlpha()
    local params = lost.gl.Texture.Params()
    self._checkmarkTexture = lost.gl.Texture.create(bmp, params)
  end
  return self._checkmarkTexture
end

function Pebble:windowResizeTexture()
  if not self._windowResizeTexture then
    local data = tasklet.loader:load("lost/guiro/themes/resize.png")
    local bmp = lost.bitmap.Bitmap.create(data)
    bmp:premultiplyAlpha()
    local params = lost.gl.Texture.Params()
    self._windowResizeTexture = lost.gl.Texture.create(bmp, params)
  end
  return self._windowResizeTexture
end

function Pebble:buttonCheckboxCandy(target, args)
  local l = lost.guiro.layer.Layer
  local rr = lost.guiro.layer.RoundedRect
  local img = lost.guiro.layer.Image

  local b = lost.guiro.view.Button
  local size = args.size or "small"
  target:mode("toggle")
  -- target bounds must have been set by now so we can modify height
  local s = self.checkboxSizes[size]
  target._bounds.height = lost.guiro.Bounds.decodeEntry(4,s)
  local r = self.checkboxRadius
  local io = 2 -- image offset
  local dc = .7
  local dimColor = Color(dc,dc,dc)
  local normal = l{sublayers={rr{bounds={0,0,s,s},gradient="candyGray",filled=true,radius=r,},
                              rr{bounds={0,0,s,s},gradient="candyGrayFrame",filled=false,radius=r}}
                             }
  local pushed = l{sublayers={rr{bounds={0,0,s,s},gradient="candyGray",color=dimColor,filled=true,radius=r,},
                              rr{bounds={0,0,s,s},gradient="candyGrayFrame",filled=false,radius=r}}
                             }

  local normal2 = l{sublayers={rr{bounds={0,0,s,s},gradient="candyBlue",filled=true,radius=r},
                               rr{bounds={0,0,s,s},gradient="candyBlueFrame",filled=false,radius=r},
                               img{bounds={io,io,s,s},texture=self:checkmarkTexture(),flip=true,scale="aspect",filter=true}}
                              }
  local pushed2 = l{sublayers={rr{bounds={0,0,s,s},gradient="candyBlue",color=dimColor,filled=true,radius=r},
                              rr{bounds={0,0,s,s},gradient="candyBlueFrame",filled=false,radius=r},
                              img{bounds={io,io,s,s},texture=self:checkmarkTexture(),flip=true,scale="aspect",filter=true}}
                             }

  local d = s + self.checkboxSpacing
  local text = lost.guiro.layer.Text{bounds={d,0,{"1",-d},"1"},font=self.checkboxFonts[size],color=Color(0,0,0),halign="left",valign="center"}
  target.layer:addSublayer(normal)
  target.layer:addSublayer(pushed)
  target.layer:addSublayer(normal2)
  target.layer:addSublayer(pushed2)
  target.layer:addSublayer(text)
  target.textLayer = text
  target.backgrounds[b.STATE_NORMAL] = normal
  target.backgrounds[b.STATE_PUSHED] = pushed
  target.backgrounds[b.STATE_NORMAL2] = normal2
  target.backgrounds[b.STATE_PUSHED2] = pushed2
  target.titleColors[b.STATE_NORMAL] = Color(0,0,0)
  target.titleColors[b.STATE_DISABLED] = Color(.8,.8,.8)  
end

function Pebble:buttonWindowClose(target,args)
  local l = lost.guiro.layer.Layer
  local d = lost.guiro.layer.Disc

  local b = lost.guiro.view.Button
  target:mode("normal")

  local dc = .7
  local dimColor = Color(dc,dc,dc)
  local normal = l{sublayers={d{bounds={0,0,"1","1"},gradient="closeButtonFill",filled=true},
                              d{bounds={0,0,"1","1"},gradient="closeButtonFrame",filled=false}}
                             }
  local pushed = l{sublayers={d{bounds={0,0,"1","1"},gradient="closeButtonFill",color=dimColor,filled=true},
                              d{bounds={0,0,"1","1"},gradient="closeButtonFrame",color=dimColor,filled=false}}
                             }

  target.layer:addSublayer(normal)
  target.layer:addSublayer(pushed)
  target.backgrounds[b.STATE_NORMAL] = normal
  target.backgrounds[b.STATE_PUSHED] = pushed
end

-- style for header of a normal window, gray, top rounded, bottom square, dark gray separator line at bottom
function Pebble:viewWindowHeader(target, args)
  target.layer:addSublayer(lost.guiro.layer.RoundedRect{bounds={0,0,"1","1"},radius=4,gradient="toolbarBg",roundCorners={tl=true, bl=false, br=false, tr=true}})
  target.layer:addSublayer(lost.guiro.layer.HLine{bounds={0,0,"1",1},color=self.separatorColor})    
  target._bounds.height = lost.guiro.Bounds.decodeEntry(4,self.windowNormalHeaderHeight)
end

-- style for header of a normal window, gray, top rounded, bottom square, dark gray separator line at bottom
function Pebble:viewPanelHeader(target, args)
  target.layer:addSublayer(lost.guiro.layer.Rect{bounds={0,0,"1","1"},radius=4,gradient="toolbarBg"})
  target.layer:addSublayer(lost.guiro.layer.HLine{bounds={0,0,"1",1},color=self.separatorColor})    
  target._bounds.height = lost.guiro.Bounds.decodeEntry(4,self.windowPanelHeaderHeight)
end


function Pebble:viewWindowBack(target, args)
  target.layer:addSublayer(lost.guiro.layer.Rect{bounds={0,0,"1","1"},color = Color(.9294,.9294,.9294),filled = true})  
  target.layer:addSublayer(lost.guiro.layer.RoundedRect{filled=false,bounds={0,0,"1","1"},color=Color(.8,.8,.8),roundCorners={tl=false, bl=false, br=false, tr=false},sides={top=false}})
end

function Pebble:windowResizeView()
  return lost.guiro.view.View{bounds={{"right",-1},1,11,11},sublayers={lost.guiro.layer.Image{bounds={0,0,11,11},texture=self:windowResizeTexture(),flip=true,scale="none"}}}  
end

function Pebble:windowNormal(target, args)
  local headerView = lost.guiro.view.View{id="header",theme="pebble", style="windowHeader",bounds={0,"top","1",self.windowNormalHeaderHeight}}
  local contentView = lost.guiro.view.View{id="content",theme="pebble", style="windowBack",clip=true,bounds={0,0,"1",{"1",-self.windowNormalHeaderHeight}}}
  local titleLabel = lost.guiro.view.Label{font={"Vera",12},color=Color(0,0,0),bounds={0,0,"1","1"},text="Window"}
  headerView:addSubview(titleLabel)
  if args.closeButton then
    local s = self.windowNormalCloseButtonSize
    local closeButton = lost.guiro.view.Button{theme="pebble", style="windowCloseButton",bounds={self.windowCloseButtonSpacing,"center",s,s}}
    headerView:addSubview(closeButton)
    target.closeButton = closeButton
  end
  target:addSubview(headerView,true)
  target:addSubview(contentView,true)
  target.headerView = headerView
  target.contentView = contentView -- set contentView after all other views were added, because when contentView is set, all following addSubview calls will redirect to this view
  target.titleLabel = titleLabel
  if args.resizable then
    local rv = self:windowResizeView()
    target:addSubview(rv, true)
    target.resizeView = rv
  end
end

function Pebble:windowPanel(target, args)
  local headerView = lost.guiro.view.View{id="header",theme="pebble", style="panelHeader",bounds={0,"top","1",self.windowPanelHeaderHeight}}
  local contentView = lost.guiro.view.View{id="content",theme="pebble", style="windowBack",clip=true,bounds={0,0,"1",{"1",-self.windowPanelHeaderHeight}}}
  local titleLabel = lost.guiro.view.Label{font={"Vera",10},color=Color(0,0,0),bounds={0,0,"1","1"},text="Window"}
  headerView:addSubview(titleLabel)
  if args.closeButton then
    local s = self.panelCloseButtonSize
    local closeButton = lost.guiro.view.Button{theme="pebble", style="windowCloseButton",bounds={self.panelCloseButtonSpacing,"center",s,s}}
    headerView:addSubview(closeButton)
    target.closeButton = closeButton
  end
  target:addSubview(headerView)
  target:addSubview(contentView)
  target.headerView = headerView
  target.contentView = contentView -- set contentView after all other views were added, because when contentView is set, all following addSubview calls will redirect to this view
  target.titleLabel = titleLabel
  if args.resizable then
    local rv = self:windowResizeView()
    target:addSubview(rv, true)
    target.resizeView = rv
  end
end

function Pebble:layerRoundedRectRecess()
  local c = 171/255
  return lost.guiro.layer.Layer
  {
    sublayers=
    {
      lost.guiro.layer.RoundedRect
      {
        gradient="recess",
        radius=4,
        filled=true
      },
      lost.guiro.layer.RoundedRect
      {
        color=Color(c,c,c),
        radius=4,
        filled=false
      }
    }
  }
end

function Pebble:layerTabviewRectRecess()
  local c = 171/255
  return lost.guiro.layer.Layer
  {
    sublayers=
    {
      lost.guiro.layer.Rect
      {
        gradient="recess",
        filled=true
      },
      lost.guiro.layer.HLine
      {
        color=Color(c,c,c),
        bounds={0,"top","1",1}
      }
    }
  }
end


function Pebble:box(target,args)
  local xoff = 8
  local titleHeight = 12
  local contentInset = 4
  local label = lost.guiro.view.Label{bounds={xoff, "top","1",titleHeight},theme="pebble", style="default",font={"Vera",10},halign="left",valign="center"}
  local contentView = lost.guiro.view.View{bounds={contentInset,contentInset,{"1",-2*contentInset},{"1",-(titleHeight+2*contentInset)}}}
  local recessLayer = self:layerRoundedRectRecess()
  recessLayer._bounds.height = lost.guiro.Bounds.decodeEntry(4,{"1",-titleHeight})
  target.layer:addSublayer(recessLayer)
  target:addSubview(label,true)
  target:addSubview(contentView,true)
  target.titleLabel = label
  target.contentView = contentView
end

function Pebble:tabviewCandyRoundedRecess(target,args)
  local sz = args.size or "regular"
  local tabbarHeight = self.buttonRoundedHeight[sz]
  local tabbar  = lost.guiro.view.TabBar{theme="pebble", style="candy",bounds={0,"top","1",tabbarHeight},itemWidth="fit", size=sz}
  local ci = 4 -- contentInset
  local contentView = lost.guiro.view.View{bounds={ci,ci,{"1",-(2*ci)}
                                                        ,{"1",-((2*ci)+(tabbarHeight/2))}
                                                        }}
  local recessedLayer = self:layerRoundedRectRecess()
  recessedLayer._bounds.height = lost.guiro.Bounds.decodeEntry(4,{"1",-(tabbarHeight/2)})
  target.layer:addSublayer(recessedLayer)
  target:addSubview(tabbar)
  target:addSubview(contentView)
  target.tabbarView = tabbar
  target.contentView = contentView
end

function Pebble:tabviewCandyRecess(target,args)
  local sz = args.size or "regular"
  local tabbarHeight = self.buttonRoundedHeight[sz]
  local tabbar  = lost.guiro.view.TabBar{theme="pebble", style="candy",bounds={0,"top","1",tabbarHeight},itemWidth="fit", size=sz}
  local ci = 4 -- contentInset
  local contentView = lost.guiro.view.View{bounds={ci,ci,{"1",-(2*ci)}
                                                        ,{"1",-((2*ci)+tabbarHeight)}
                                                        }}
  local recessedLayer = self:layerTabviewRectRecess()
  recessedLayer._bounds.height = lost.guiro.Bounds.decodeEntry(4,{"1",-(tabbarHeight/2)})
  target.layer:addSublayer(recessedLayer)
  target:addSubview(tabbar)
  target:addSubview(contentView)
  target.tabbarView = tabbar
  target.contentView = contentView
end

function Pebble:tabviewSquare(target,args)
  local sz = args.size or "regular"
  local tabbarHeight = self.buttonRoundedHeight[sz]
  local tabbar  = lost.guiro.view.TabBar{theme="pebble", style="square",bounds={0,"top","1",tabbarHeight},itemWidth="equal", size=sz}
  local contentView = lost.guiro.view.View{bounds={0,0,"1",{"1",-tabbarHeight}}}
  target:addSubview(tabbar)
  target:addSubview(contentView)
  target.tabbarView = tabbar
  target.contentView = contentView
end

function Pebble:sliderCandy(target,args)
  local l = lost.guiro.layer.Layer
  local d = lost.guiro.layer.Disc
  local rr = lost.guiro.layer.RoundedRect

  local sz = args.size or "regular"

  target.mode = args.mode or "horizontal"

  local ts = self.sliderCandyTrackSize[sz]
  local hs = self.sliderCandyHandleSize[sz]
  target.handleSize=hs
  local trackRadius = 2
  local dc = .7
  local dimColor = Color(dc, dc, dc)
  local trackFillColor = Color(.4,.4,.4)
  local trackFrameColor = Color(.1,.1,.1)
  
  local handleReleased = l{bounds={0,0,hs,hs},
                           sublayers={d{bounds={0,0,"1","1"},gradient="candyBlue",filled=true,},
                                      d{bounds={0,0,"1","1"},gradient="candyBlueFrame",filled=false}}}

  local handlePushed = l{bounds={0,0,hs,hs},
                           sublayers={d{bounds={0,0,"1","1"},color=dimColor,gradient="candyBlue",filled=true,},
                                      d{bounds={0,0,"1","1"},color=dimColor,gradient="candyBlueFrame",filled=false}}}
  local tw = 0
  local th = 0
  if target.mode == "horizontal" then
    tw="1"
    th=ts
--    target:height(ts)
  else
    tw=ts
    th="1"
--    target:width(ts)
  end
  local track = l{bounds={0,0,tw,th},
                  sublayers={
                    rr{filled=true,radius=trackRadius,color=trackFillColor},
--                    rr{filled=false,radius=trackRadius,color=trackFrameColor,width=1},
                  }}
  
  if target.mode == "horizontal" then
    track:y("center")
    handleReleased:y("center")
    handlePushed:y("center")
    target.layer:addSublayer(track)
    target.layer:addSublayer(handleReleased)
    target.handleReleasedLayer = handleReleased
    target.layer:addSublayer(handlePushed)
    target.handlePushedLayer = handlePushed
  else
    track:x("center")
    handleReleased:x("center")
    handlePushed:x("center")
    target.layer:addSublayer(track)
    target.layer:addSublayer(handleReleased)
    target.handleReleasedLayer = handleReleased
    target.layer:addSublayer(handlePushed)
    target.handlePushedLayer = handlePushed
  end
end
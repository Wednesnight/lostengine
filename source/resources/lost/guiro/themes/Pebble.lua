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
local Bounds = lost.guiro.Bounds

--local rr = lost.guiro.layer.RoundedRect
local rr = lost.guiro.layer.RoundedRect

function Pebble:constructor()
  lost.guiro.Theme.constructor(self)
  pebbleBuildGradients(lost.guiro.textureManager())
  self.name = "pebble"
  self:addStyle("lost.guiro.view.Label", "default", function(target, args) self:labelDefault(target, args) end)
  self:addStyle("lost.guiro.view.Label", "round", function(target, args) self:labelRound(target, args) end)
  self:addStyle("lost.guiro.view.Label", "roundFramed", function(target, args) self:labelRoundFramed(target, args) end)
  self:addStyle("lost.guiro.view.Label", "cplabel", function(target, args) self:labelColorPicker(target, args) end)
  self:addStyle("lost.guiro.view.Button", "default", function(target, args) self:buttonRounded(target, args) end)
  self:addStyle("lost.guiro.view.Button", "rounded", function(target, args) self:buttonRounded(target, args) end)
  self:addStyle("lost.guiro.view.Button", "roundedToggle", function(target, args) self:buttonRoundedToggle(target, args) end)
  self:addStyle("lost.guiro.view.View", "default", function(target, args)  end)
  self:addStyle("lost.guiro.view.View", "gray", function(target, args) self:viewGray(target, args) end)
  self:addStyle("lost.guiro.view.View", "toolbar", function(target, args) self:viewToolbar(target, args) end)
  self:addStyle("lost.guiro.view.UserInterface", "default", function(target, args)  end)

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
  self:addStyle("lost.guiro.view.Slider", "scrollbar", function(target, args) self:sliderScrollBar(target, args) end)

  self:addStyle("lost.guiro.view.ColorPicker", "default", function(target, args) self:colorPicker(target, args) end)
  self:addStyle("lost.guiro.view.ColorPickerWindow", "default", function(target, args) self:colorPickerWindow(target, args) end)

  self:addStyle("lost.guiro.view.FpsMeter", "default", function(target, args) self:fpsMeter(target, args) end)

  self:addStyle("lost.guiro.view.TextInput", "default", function(target, args) self:textInput(target, args) end)
  self:addStyle("lost.guiro.view.Image", "default", function(target, args) self:imageView(target, args) end)
  self:addStyle("lost.guiro.view.Image", "roundCell", function(target, args) self:imageRoundCell(target, args) end)

  self:addStyle("lost.guiro.view.MenuBar", "default", function(target, args) self:menuBar(target, args) end)
  self:addStyle("lost.guiro.view.MenuBarItem", "default", function(target, args) self:menuBarItem(target, args) end)
  self:addStyle("lost.guiro.view.Menu", "default", function(target, args) self:menuRoundRect(target, args) end)
  self:addStyle("lost.guiro.view.Menu", "toprect", function(target, args) self:menuTopRect(target, args) end)
  self:addStyle("lost.guiro.view.Menu", "roundrect", function(target, args) self:menuRoundRect(target, args) end)
  self:addStyle("lost.guiro.view.MenuItem", "default", function(target, args) self:menuItem(target, args) end)

  self:addStyle("lost.guiro.view.PopUpButton", "candy", function(target, args) self:popUpButtonCandy(target, args) end)
  self:addStyle("lost.guiro.view.PopUpButton", "default", function(target, args) self:popUpButtonCandy(target, args) end)

  self:addStyle("lost.guiro.view.ScrollBar", "default", function(target, args) self:scrollBar(target, args) end)
  self:addStyle("lost.guiro.view.ScrollView", "default", function(target, args) self:scrollView(target, args) end)

  self:addStyle("lost.guiro.view.ListView", "default", function(target, args) self:listView(target, args) end)

  self:addStyle("lost.guiro.view.Button", "scrollbarUp", function(target, args) self:buttonScrollbarUp(target, args) end)
  self:addStyle("lost.guiro.view.Button", "scrollbarDown", function(target, args) self:buttonScrollbarDown(target, args) end)
  self:addStyle("lost.guiro.view.Button", "scrollbarLeft", function(target, args) self:buttonScrollbarLeft(target, args) end)
  self:addStyle("lost.guiro.view.Button", "scrollbarRight", function(target, args) self:buttonScrollbarRight(target, args) end)

  self:addStyle("lost.guiro.view.DirectoryView", "default", function(target, args) self:directoryView(target, args) end)

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
  self.windowNormalCloseButtonSize = 12
  self.panelCloseButtonSize = 9
  self.windowCloseButtonSpacing = 8 --- distance to left window border
  self.panelCloseButtonSpacing = 6
  
  self.squareButtonLightFrameCol = Color(168/255, 168/255, 168/255)
  self.squareButtonDarkFrameCol = Color(145/255, 145/255, 145/255)
  
  self.sliderCandyTrackSize={regular=6, small=4,mini=2}
  self.sliderCandyHandleSize={regular=16, small=12,mini=10}
  
--[[  self.menuBarHeight = 22
  self.menuBarItemFont = {"Vera", 12}
  self.menuRadius = 6
--  self.menuColor = Color(244/255, 244/255, 244/255,.8)
  self.menuColor = Color(1, 1, 1,.95)
  self.menuSeparatorColor = Color(218/255,218/255,218/255)
  self.menuFrameColor = Color(0,0,0,.2)
]]
  self.menuColor = Color(1, 1, 1,.93)
  self.menuFrameColor = Color(0,0,0,.2)
  self.menuSeparatorColor = Color(218/255,218/255,218/255)
  self.menuAccessoireColor = Color(.5,.5,.5)
  self.menuBarItemTextColor = Color(0,0,0)
  self.menuBarItemHighlightTextColor = Color(1,1,1)
  self.menuItemTextColor = Color(0,0,0)
  self.menuItemHighlightTextColor = Color(1,1,1)

  self.menuParams = 
  {
    regular =
    {
      barHeight = 22,
      barItemFont = {"Vera",12},
      barItemPadding = 20,
      barItemLeftOffset = 10,
      itemFont = {"Vera",12},
      radius = 6,
      color=menuColor,
      separatorHeight = 11,
      checkmarkWidth = 12,
      submenuWidth = 16,
      menuItemHeight=22,
      checkmarkTextDistance=4,
      textSubmenuDistance=4,
      menuTopMargin = 4,
      menuBottomMargin = 4,
      menuLeftMargin = 4,
      menuRightMargin = 4,
    },
    small=
    {
      barHeight = 18,
      barItemFont = {"Vera",10},
      barItemPadding = 20,
      barItemLeftOffset = 10,
      itemFont = {"Vera",10},
      radius = 6,
      color=menuColor,
      separatorHeight = 9,
      checkmarkWidth = 10,
      submenuWidth = 14,
      menuItemHeight=18,
      checkmarkTextDistance=4,
      textSubmenuDistance=4,
      menuTopMargin = 4,
      menuBottomMargin = 4,
      menuLeftMargin = 4,
      menuRightMargin = 4,
    },
    mini=
    {
      barHeight = 16,
      barItemFont = {"Vera",8},
      barItemPadding = 20,
      barItemLeftOffset = 10,
      itemFont = {"Vera",8},
      radius = 6,
      color=menuColor,
      separatorHeight = 7,
      checkmarkWidth = 10,
      submenuWidth = 12,
      menuItemHeight=16,
      checkmarkTextDistance=4,
      textSubmenuDistance=4,
      menuTopMargin = 4,
      menuBottomMargin = 4,
      menuLeftMargin = 4,
      menuRightMargin = 4,
    }
  }
  
  self.popUpButtonParams=
  {
    candy=
    {
      regular=
      {
        font={"Vera", 12},
        height=22,
        radius=3,
        rightSectionWidth = 21,
        imageSize = {10,13},
        imageOffset={-1,1},
      },
      small=
      {
        font={"Vera", 10},
        height=18,
        radius=3,
        rightSectionWidth = 18,
        imageSize = {10,13},
        imageOffset={-1,1},
      },
      mini=
      {
        font={"Vera", 9},
        height=15,
        radius=3,
        rightSectionWidth = 14,
        imageSize = {9,12},
        imageOffset={-1,1},
      }
    },
  }
  
  self.scrollbarArrowColor = Color(86/255, 86/255, 86/255)
  self.scrollbarArrowColorPushed = Color(0,0,0)
  self.scrollbarFrameColor = Color(171/255, 171/255, 171/255)
  self.scrollbarTrackColor = Color(230/255, 230/255, 230/255)
  self.scrollbarArrowInset = {regular=6, small=4, mini=2}
  self.scrollbarWidth = {regular=16, small=12, mini=10}
  self.scrollbarHandleRadius = {regular=6,small=4,mini=3}
  self.scrollbarButtonSize={regular=18,small=14,mini=12}
end

function Pebble:labelDefault(target, args)
  target.layer:addSublayer(lost.guiro.layer.Text{bounds={0,0,"1","1"},color=Color(0,0,0),font = {"Vera", 12}})
end

function Pebble:labelRound(target, args)
  target.layer:addSublayer(rr{bounds={0,0,"1","1"},color=Color(1,1,1),radius=8,filled=true})
  target.layer:addSublayer(lost.guiro.layer.Text{bounds={0,0,"1","1"},color = Color(0,0,0),font = {"Vera", 12}})
end

function Pebble:labelColorPicker(target, args)
  target.layer:addSublayer(rr{bounds={0,0,"1","1"},color=Color(1,1,1),radius=4,filled=true})
  target.layer:addSublayer(rr{bounds={0,0,"1","1"},color=Color(0,0,0),radius=4,filled = false,width=1})
  target.layer:addSublayer(lost.guiro.layer.Text{bounds={0,0,"1","1"},color = Color(0,0,0),font = {"Vera", 12}})
end


function Pebble:labelRoundFramed(target, args)
  target.layer:addSublayer(rr{bounds={0,0,"1","1"},color=Color(1,1,1),radius=8,filled=true})
  target.layer:addSublayer(rr{bounds={0,0,"1","1"},color=Color(0,0,0),radius=8,filled = false,width=1})
  target.layer:addSublayer(lost.guiro.layer.Text{bounds={0,0,"1","1"},color=Color(0,0,0),font={"Vera", 12}})
end

function Pebble:buttonRounded(target,args)
  local l = lost.guiro.layer.Layer

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
  
  target.buttonSizeAdjust = args.buttonSizeAdjust or 30
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
  
  target.buttonSizeAdjust = args.buttonSizeAdjust or 30
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

function Pebble:submenuarrowTexture()
  if not self._submenuarrowTexture then
    local data = tasklet.loader:load("lost/guiro/themes/submenuarrow.png")
    local bmp = lost.bitmap.Bitmap.create(data)
    bmp:premultiplyAlpha()
    local params = lost.gl.Texture.Params()
    self._submenuarrowTexture = lost.gl.Texture.create(bmp, params)
  end
  return self._submenuarrowTexture
end

function Pebble:popuparrowTexture()
  if not self._popuparrowTexture then
    local data = tasklet.loader:load("lost/guiro/themes/popupArrows.png")
    local bmp = lost.bitmap.Bitmap.create(data)
    bmp:premultiplyAlpha()
    local params = lost.gl.Texture.Params()
    self._popuparrowTexture = lost.gl.Texture.create(bmp, params)
  end
  return self._popuparrowTexture
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

function Pebble:varrowTexture()
  if not self._varrowTexture then
    local data = tasklet.loader:load("lost/guiro/themes/varrow.png")
    local bmp = lost.bitmap.Bitmap.create(data)
    bmp:premultiplyAlpha()
    local params = lost.gl.Texture.Params()
    self._varrowTexture = lost.gl.Texture.create(bmp, params)
  end
  return self._varrowTexture
end

function Pebble:rarrowTexture()
  if not self._rarrowTexture then
    local data = tasklet.loader:load("lost/guiro/themes/rarrow.png")
    local bmp = lost.bitmap.Bitmap.create(data)
    bmp:premultiplyAlpha()
    local params = lost.gl.Texture.Params()
    self._rarrowTexture = lost.gl.Texture.create(bmp, params)
  end
  return self._rarrowTexture
end

function Pebble:larrowTexture()
  if not self._larrowTexture then
    local data = tasklet.loader:load("lost/guiro/themes/larrow.png")
    local bmp = lost.bitmap.Bitmap.create(data)
    bmp:premultiplyAlpha()
    local params = lost.gl.Texture.Params()
    self._larrowTexture = lost.gl.Texture.create(bmp, params)
  end
  return self._larrowTexture
end

function Pebble:directoryTexture()
  if not self._directoryTexture then
    local data = tasklet.loader:load("lost/guiro/themes/directory.png")
    local bmp = lost.bitmap.Bitmap.create(data)
    bmp:premultiplyAlpha()
    local params = lost.gl.Texture.Params()
    self._directoryTexture = lost.gl.Texture.create(bmp, params)
  end
  return self._directoryTexture
end

function Pebble:fileTexture()
  if not self._fileTexture then
    local data = tasklet.loader:load("lost/guiro/themes/file.png")
    local bmp = lost.bitmap.Bitmap.create(data)
    bmp:premultiplyAlpha()
    local params = lost.gl.Texture.Params()
    self._fileTexture = lost.gl.Texture.create(bmp, params)
  end
  return self._fileTexture
end

function Pebble:buttonCheckboxCandy(target, args)
  local l = lost.guiro.layer.Layer
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
                               img{bounds={io,io,s,s},texture=self:checkmarkTexture(),flip=true,color=Color(0,0,0),scale="aspect",filter=true}}
                              }
  local pushed2 = l{sublayers={rr{bounds={0,0,s,s},gradient="candyBlue",color=dimColor,filled=true,radius=r},
                              rr{bounds={0,0,s,s},gradient="candyBlueFrame",filled=false,radius=r},
                              img{bounds={io,io,s,s},texture=self:checkmarkTexture(),flip=true,color=Color(0,0,0),scale="aspect",filter=true}}
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
  target.layer:addSublayer(rr{bounds={0,0,"1","1"},radius=4,gradient="toolbarBg",roundCorners={tl=true, bl=false, br=false, tr=true}})
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
  target.layer:addSublayer(rr{filled=false,bounds={0,0,"1","1"},color=Color(.8,.8,.8),roundCorners={tl=false, bl=false, br=false, tr=false},sides={top=false}})
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
      rr
      {
        gradient="recess",
        radius=4,
        filled=true
      },
      rr
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
  local bsa = args.buttonSizeAdjust
  local tabbar  = lost.guiro.view.TabBar{theme="pebble", style="candy",bounds={0,"top","1",tabbarHeight},itemWidth="fit", size=sz,buttonSizeAdjust=bsa}
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
  local bsa = args.buttonSizeAdjust
  local tabbar  = lost.guiro.view.TabBar{theme="pebble", style="candy",bounds={0,"top","1",tabbarHeight},itemWidth="fit", size=sz,buttonSizeAdjust=bsa}
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
  local bsa = args.buttonSizeAdjust
  local tabbar  = lost.guiro.view.TabBar{theme="pebble", style="square",bounds={0,"top","1",tabbarHeight},itemWidth="equal", size=sz,buttonSizeAdjust=bsa}
  local contentView = lost.guiro.view.View{bounds={0,0,"1",{"1",-tabbarHeight}}}
  target:addSubview(tabbar)
  target:addSubview(contentView)
  target.tabbarView = tabbar
  target.contentView = contentView
end

function Pebble:sliderCandy(target,args)
  local l = lost.guiro.layer.Layer
  local d = lost.guiro.layer.Disc

  local sz = args.size or "regular"

  target.mode = args.mode or "horizontal"

  local ts = self.sliderCandyTrackSize[sz]
  local hs = self.sliderCandyHandleSize[sz]
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
  else
    tw=ts
    th="1"
  end
  local track = l{bounds={0,0,tw,th},
                  sublayers={
                    rr{filled=true,radius=trackRadius,color=trackFillColor},
                  }}
  
  if target.mode == "horizontal" then
    track:y("center")
    handleReleased:y("center")
    handlePushed:y("center")
  else
    track:x("center")
    handleReleased:x("center")
    handlePushed:x("center")
  end
  target.layer:addSublayer(track)
  target.layer:addSublayer(handleReleased)
  target.handleReleasedLayer = handleReleased
  target.layer:addSublayer(handlePushed)
  target.handlePushedLayer = handlePushed
  target:handleSize(hs)  
end

function Pebble:colorPicker(target, args)
  local fc = 151/255
  local frameColor = Color(fc, fc, fc)
  local ifc = 131/255
  local innerFrameColor = Color(ifc, ifc, ifc)
  local dc = .7
  local dimColor = Color(dc, dc, dc)
  local dc2 = .8
  local dimColor2 = Color(dc2, dc2, dc2)
  
  local l = lost.guiro.layer.Layer
  local r = lost.guiro.layer.Rect

  target:mode("toggleOnce")
  local normal = l{sublayers={r{bounds={0,0,"1","1"},color=frameColor,filled=false},
                              r{bounds={1,1,{"1",-2},{"1",-2}},gradient="colorpicker",filled=true}}
                             }

  local pushed = l{sublayers={r{bounds={0,0,"1","1"},color=frameColor,filled=false},
                              r{bounds={1,1,{"1",-2},{"1",-2}},gradient="colorpicker",color=dimColor,filled=true}}
                              }
                             
  local normal2 = l{sublayers={r{bounds={0,0,"1","1"},color=frameColor,filled=false},
                              r{bounds={1,1,{"1",-2},{"1",-2}},gradient="colorpicker",color=dimColor2,filled=true}}
                              }

  target.layer:addSublayer(normal)
  target.layer:addSublayer(pushed)
  target.layer:addSublayer(normal2)
  local b = lost.guiro.view.Button
  target.backgrounds[b.STATE_NORMAL] = normal
  target.backgrounds[b.STATE_PUSHED] = pushed
  target.backgrounds[b.STATE_NORMAL2] = normal2  

  -- the actual color display must always be visible and is added last on top of all the state layers
  local colorLayerFrame = r{bounds={5,5,{"1",-10},{"1",-10}},filled=false,color=innerFrameColor}
  local colorLayer = r{bounds={6,6,{"1",-12},{"1",-12}},filled=true,color=Color(1,1,1)}
  target.layer:addSublayer(colorLayerFrame)
  target.layer:addSublayer(colorLayer)
  target.colorLayer = colorLayer
  target.windowTheme = "pebble"
  target.windowStyle = "default"
end

function Pebble:colorPickerWindow(target, args)
  args.closeButton = true
  args.resizable = true
  self:windowPanel(target, args)
  if not args.bounds then
    target:bounds(lost.guiro.Bounds(50,{"top",-50},260,160))
  else
    target:bounds(lost.guiro.Bounds(unpack(args.bounds)))
  end
  
  -- names shortened so layer definitions don't get too long
  local ci = 4 -- content inset
  local h = 16 -- height
  local sp = 4 -- spacing
  local ss = "regular" -- slider size
  local lw = 30 -- labelwidth
  local clw = 30

  -- sliders
  local rs = lost.guiro.view.Slider{size=ss,min=0,max=1,bounds={ci+clw+sp,{"top",-ci},{"1",-(2*ci+2*sp+lw+clw)},h}}
  target:addSubview(rs)
  target.redSlider = rs
  local gs = lost.guiro.view.Slider{size=ss,min=0,max=1,bounds={ci+clw+sp,{"top",-(ci+h+sp)},{"1",-(2*ci+2*sp+lw+clw)},h}}
  target:addSubview(gs)
  target.greenSlider = gs
  local bs = lost.guiro.view.Slider{size=ss,min=0,max=1,bounds={ci+clw+sp,{"top",-(ci+2*h+2*sp)},{"1",-(2*ci+2*sp+lw+clw)},h}}
  target:addSubview(bs)
  target.blueSlider = bs
  local as = lost.guiro.view.Slider{size=ss,min=0,max=1,bounds={ci+clw+sp,{"top",-(ci+3*h+3*sp)},{"1",-(2*ci+2*sp+lw+clw)},h}}
  target:addSubview(as)
  target.alphaSlider = as

  -- value labels
  local rl = lost.guiro.view.Label{style="cplabel", bounds={{"right",-ci}, {"top",-(ci+0*h+0*sp)},lw,h}, valign="center", halign="center"}
  target:addSubview(rl)
  target.redLabel = rl
  local gl = lost.guiro.view.Label{style="cplabel", bounds={{"right",-ci}, {"top",-(ci+1*h+1*sp)},lw,h}, valign="center", halign="center"}
  target:addSubview(gl)
  target.greenLabel = gl
  local bl = lost.guiro.view.Label{style="cplabel", bounds={{"right",-ci}, {"top",-(ci+2*h+2*sp)},lw,h}, valign="center", halign="center"}
  target:addSubview(bl)
  target.blueLabel = bl
  local al = lost.guiro.view.Label{style="cplabel", bounds={{"right",-ci}, {"top",-(ci+3*h+3*sp)},lw,h}, valign="center", halign="center"}
  target:addSubview(al)
  target.alphaLabel = al
  
  -- color labels
  target.contentView.layer:addSublayer(rr{bounds={ci,{"top",-(ci+0*h+0*sp)},clw,h},filled=true,color=Color(1,0,0),radius=4})
  target.contentView.layer:addSublayer(rr{bounds={ci,{"top",-(ci+1*h+1*sp)},clw,h},filled=true,color=Color(0,1,0),radius=4})
  target.contentView.layer:addSublayer(rr{bounds={ci,{"top",-(ci+2*h+2*sp)},clw,h},filled=true,color=Color(0,0,1),radius=4})
  target.contentView.layer:addSublayer(rr{bounds={ci,{"top",-(ci+3*h+3*sp)},clw,h},filled=false,width=1,color=Color(0,0,0),radius=4})
  
  -- colorLayer for better visualisation of current setting
  target.contentView.layer:addSublayer(lost.guiro.layer.Rect{
      filled=false,
      color=Color(0,0,0), 
      bounds={ci,
              ci,
              {"1",-2*ci},
              {"1",-(2*ci+4*h+4*sp)}
              }
      } 
  )
  local colorLayer = lost.guiro.layer.Rect{
      filled=true,
      color=Color(1,1,1), 
      bounds={ci+1,
              ci+1,
              {"1",-2*(ci+1)},
              {"1",-(2*(ci+1)+4*h+4*sp)}
              }
      }
  target.contentView.layer:addSublayer(colorLayer)
  target.colorLayer = colorLayer
end

function Pebble:fpsMeter(target, args)
  target.layer:addSublayer(rr{filled=true, radius=8,color=Color(0,0,0,.6)})
  local tl = lost.guiro.layer.Text{font={"Vera mono bold",48},color=Color(1,1,1),valign="center",halign="center"}
  target.layer:addSublayer(tl)
  target.textLayer = tl
end

function Pebble:textInput(target, args)
  local t = lost.guiro.layer.Text
  local r = lost.guiro.layer.Rect
  local focusColor = Color(.4,.6,1)
  local selectionColor = Color(.4,.6,1,.2)
  local focusRing = r{hidden=true,bounds={0,0,"1","1"},filled=false,color=focusColor,width=2}
  local tmarg=2
  -- bounds={tmarg,tmarg,{"1",-2*tmarg},{"1",-2*tmarg}}
  local textLayer = t{characterMetrics=true,clip=true,bounds={tmarg,tmarg,{"1",-2*tmarg},{"1",-2*tmarg}},font={"Vera",12},color=Color(0,0,0)}
  local cursorLayer = r{bounds={0,0,2, textLayer._font.lineHeight}, color=Color(.3,.3,.3,.5),filled=true}
  local selectionLayer1 = r{bounds={0,0,0,0}, color=selectionColor,filled=true,hidden=true}
  local selectionLayer2 = r{bounds={0,0,0,0}, color=selectionColor,filled=true,hidden=true}
  local selectionLayer3 = r{bounds={0,0,0,0}, color=selectionColor,filled=true,hidden=true}
  target.layer:addSublayer(r{bounds={0,0,"1","1"},filled=true,color=Color(1,1,1)})
  target.layer:addSublayer(r{bounds={0,0,"1","1"},filled=false,color=Color(0,0,0)})
  target.layer:addSublayer(textLayer)
  target.layer:addSublayer(focusRing)
  textLayer:addSublayer(cursorLayer)
  textLayer:addSublayer(selectionLayer1)
  textLayer:addSublayer(selectionLayer2)
  textLayer:addSublayer(selectionLayer3)
  textLayer.cursorLayer = cursorLayer
  textLayer.selectionLayers = {selectionLayer1,selectionLayer2,selectionLayer3}
  target.focusLayer = focusRing
  target.textLayer = textLayer
  target.cursorLayer = cursorLayer
end

function Pebble:imageView(target, args)
  local iv = lost.guiro.layer.Image{}
  target.layer:addSublayer(iv)
  target.imageLayer = iv
end

function Pebble:menuBar(target, args)
  local size = args.size or "regular"
  local mp = self.menuParams[size]

  target.layer:addSublayer(lost.guiro.layer.Rect{filled=true,color=Color(1,1,1),gradient="menubarback"})
  target:height(mp.barHeight)
  target.menuBarItemTheme="pebble"
  target.menuBarItemStyle="default"
  target.menuBarItemStyleParams = {theme="pebble", style="default", ["size"]=size}  
  target.itemPadding = mp.barItemPadding
  target.itemLeftOffset = mp.barItemLeftOffset
end

function Pebble:menuBarItem(target, args)
  local size = args.size or "regular"
  local mp = self.menuParams[size]

  local l = lost.guiro.layer.Layer
  local r = lost.guiro.layer.Rect
  local b = lost.guiro.view.Button
  
  local highlight = r{bounds={0,0,"1","1"},gradient="menubaritemselected",filled=true}
  local text = lost.guiro.layer.Text{bounds={0,0,"1","1"},valign="center", clip=true, characterMetrics=false, halign="center",font=mp.barItemFont,
                                     color=self.menuBarItemTextColor}
  target.layer:addSublayer(highlight)
  target.layer:addSublayer(text)
  target.textLayer = text
  target.highlightLayer = highlight
  target.normalTextColor = self.menuBarItemTextColor
  target.highlightedTextColor = self.menuBarItemHighlightTextColor
  target.menuTheme="pebble"
  target.menuStyle="toprect"
  target:height(mp.barHeight)
end

function Pebble:menuShared(target, args)
  local size = args.size or "regular"
  local mp = self.menuParams[size]

  local highlight = lost.guiro.layer.Rect{gradient="menubaritemselected", filled=true}
  target.layer:addSublayer(highlight)
  target.highlightLayer = highlight
  args.movable = false
  target:bounds(lost.guiro.Bounds(10,10,100,200))
  target.topMargin = mp.menuTopMargin
  target.bottomMargin = mp.menuBottomMargin
  target.leftMargin = mp.menuLeftMargin
  target.rightMargin = mp.menuRightMargin
  local hl =  lost.guiro.layer.HLine
  local l = lost.guiro.layer.Layer
  target.createSeparatorLayerFunc = function() return l{bounds={0,0,"1",mp.separatorHeight},
                                                        sublayers={hl{bounds={0,"center","1",1}, color=self.menuSeparatorColor}} 
                                                       } 
                                    end
  target.separatorHeight = mp.separatorHeight                       
end

function Pebble:menuRoundRect(target, args)
  local size = args.size or "regular"
  local mp = self.menuParams[size]

  local bg = rr{filled=true, color=self.menuColor,radius=mp.radius}
  local fr = rr{filled=false, color=self.menuFrameColor, radius=mp.radius,bounds={0,0,"1","1"}}
  target.layer:addSublayer(bg)
  target.layer:addSublayer(fr)
  self:menuShared(target, args)
end

function Pebble:menuTopRect(target, args)
  local size = args.size or "regular"
  local mp = self.menuParams[size]
  local bg = rr{filled=true, color=self.menuColor,radius=mp.radius,roundCorners={tl=false, tr=false}}
  local fr = rr{filled=false, color=self.menuFrameColor, radius=mp.radius, roundCorners={tl=false, tr=false}, sides={top=false},bounds={0,0,"1","1"}}
  target.layer:addSublayer(bg)
  target.layer:addSublayer(fr)
  self:menuShared(target, args)
end

function Pebble:menuItem(target, args)
  local size = args.size or "regular"
  local mp = self.menuParams[size]
  local tl = lost.guiro.layer.Text{font=mp.barItemFont, color=mp.itemTextColor, breakMode="none",valign="center",halign="left", clip=true}
  local fnt = tasklet.fontManager:getFont(mp.itemFont[1], mp.itemFont[2])
  local imageOffset = math.abs(fnt.descender)
  target.checkmarkWidth = mp.checkmarkWidth
  target.submenuWidth = mp.submenuWidth
  local accessoireColor = self.menuAccessoireColor
  local checkmark = lost.guiro.layer.Image{bounds={0,{"center",imageOffset},target.checkmarkWidth,target.checkmarkWidth},texture=self:checkmarkTexture(),flip=true,color=accessoireColor,scale="aspect",filter=true}  
  local submenu = lost.guiro.layer.Image{bounds={0,{"center",imageOffset},target.submenuWidth,target.submenuWidth},texture=self:submenuarrowTexture(),flip=true,color=accessoireColor,scale="aspect",filter=true}  
  target.layer:addSublayer(tl)
  target.layer:addSublayer(checkmark)
  target.layer:addSublayer(submenu)
  target.textLayer = tl
  target.checkmarkLayer = checkmark
  target.submenuLayer = submenu
  target.highlightTextColor = self.menuItemHighlightTextColor
  target.normalTextColor = self.menuItemTextColor
  
  target.contentHeight = mp.menuItemHeight
  target.checkmarkTextDistance = mp.checkmarkTextDistance
  target.textSubmenuDistance = mp.textSubmenuDistance
  checkmark:bounds(Bounds(0,"center", target.checkmarkWidth, target.checkmarkWidth))
  tl:x(target.checkmarkWidth+target.checkmarkTextDistance)
  tl:width({"1",-(target.checkmarkWidth+target.submenuWidth+target.checkmarkTextDistance+target.textSubmenuDistance)})
  submenu:x("right")
  submenu:size(target.submenuWidth, target.submenuWidth)
end

function Pebble:popUpButtonCandy(target, args)
  local size = args.size or "regular"
  local params = self.popUpButtonParams.candy[size]
  local l = lost.guiro.layer.Layer
  local i = lost.guiro.layer.Image
  local b = lost.guiro.view.Button
  target:mode("toggleOnce")
  target:height(params.height)
  local r = params.radius
  local imageSize = self.popUpButtonParams.candy[size].imageSize
  local imageOffset = self.popUpButtonParams.candy[size].imageOffset
  local rightWidth = self.popUpButtonParams.candy[size].rightSectionWidth
  local d = (rightWidth-imageSize[2])/2
  local imageBounds = {{"right",imageOffset[1]-d},
                       {"center",imageOffset[2]},
                       imageSize[1],imageSize[2]}
  local normal = l{sublayers={rr{bounds={0,0,{"1",-rightWidth},"1"},gradient="candyGray",filled=true,radius=r,sides={right=false}},
                              rr{bounds={0,0,{"1",-rightWidth},"1"},gradient="candyGrayFrame",filled=false,radius=r,sides={right=false}},
                              rr{bounds={"right",0,rightWidth,"1"},gradient="candyBlue",filled=true,radius=r,sides={left=false}},
                              rr{bounds={"right",0,rightWidth,"1"},gradient="candyBlueFrame",filled=false,radius=r,roundCorners={tl=false, bl=false}},
                              i{bounds=imageBounds, texture=self:popuparrowTexture(), color=Color(0,0,0),scale="aspect",filter=true},
                              }
                            }
  local pushed = l{sublayers={rr{bounds={0,0,"1","1"},gradient="candyGray",color=Color(.9,.9,.9),filled=true,radius=r},
                              rr{bounds={0,0,"1","1"},gradient="candyGrayFrame",filled=false,radius=r},
                              i{bounds=imageBounds, texture=self:popuparrowTexture(), color=Color(0,0,0),scale="aspect",filter=true},
                            }
                             }

  local normal2 = l{sublayers={rr{bounds={0,0,"1","1"},gradient="candyBlue",filled=true,radius=r},
                               rr{bounds={0,0,"1","1"},gradient="candyBlueFrame",filled=false,radius=r},
                               i{bounds=imageBounds, texture=self:popuparrowTexture(), color=Color(0,0,0),scale="aspect",filter=true},
                             }
                              }

  local text = lost.guiro.layer.Text{bounds={0,0,{"1",-rightWidth},"1"},font=params.font,color=Color(0,0,0),clip=true}
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
  target.menuStyleParams = {theme="pebble", style="roundrect", size=size}
end

-- handle size is different here, spans full "width" (relative) and varying "length"
function Pebble:sliderScrollBar(target, args)
  local l = lost.guiro.layer.Layer
  local r = lost.guiro.layer.Rect
  local rr = lost.guiro.layer.RoundedRect
  local sz = args.size or "regular"
  target.mode = args.mode or "horizontal"

  local dc = .7
  local dimColor = Color(dc, dc, dc)
  local trackFillColor = Color(.4,.4,.4)
  local trackFrameColor = Color(.1,.1,.1)
  local hr = self.scrollbarHandleRadius[sz]
  local gv = true
  if target.mode == "vertical" then
    gv = false
  end
  local grad = "candyBlue"
  local gradFrame = "candyBlueFrame"
  if target.mode == "vertical" then
    grad = "candyBlueMirrored"
    gradFrame = "candyBlueFrameMirrored"    
  end
  local handleReleased = l{bounds={1,1,{"1",-2},{"1",-2}},
                           sublayers={rr{bounds={0,0,"1","1"},gradient=grad,filled=true,radius=hr,gradientVertical=gv},
                                      rr{bounds={0,0,"1","1"},gradient=gradFrame,filled=false,radius=hr,gradientVertical=gv}}}

  local handlePushed = l{bounds={1,1,{"1",-2},{"1",-2}},
                           sublayers={rr{bounds={0,0,"1","1"},color=dimColor,gradient=grad,filled=true,radius=hr,gradientVertical=gv},
                                      rr{bounds={0,0,"1","1"},color=dimColor,gradient=gradFrame,filled=false,radius=hr,gradientVertical=gv}}}
  if target.mode == "horizontal" then
    handleReleased:y("center")
    handlePushed:y("center")
    target.layer:addSublayer(l{sublayers={
                                r{filled=true, gradient="squareGlass"},
                                rr{bounds={1,1,{"1",-2},{"1",-2}},filled=true,color=self.scrollbarTrackColor,gradient="squareGlassMirrored",radius=hr+1},
                              }})
  else
    handleReleased:x("center")
    handlePushed:x("center")
    target.layer:addSublayer(l{sublayers={
                                r{filled=true, gradient="squareGlassMirrored", gradientVertical=false},
                                rr{bounds={1,1,{"1",-2},{"1",-2}},filled=true,gradient="squareGlass",color=self.scrollbarTrackColor,gradientVertical=false,radius=hr+1},
                              }})
  end
  target.layer:addSublayer(handleReleased)
  target.handleReleasedLayer = handleReleased
  target.layer:addSublayer(handlePushed)
  target.handlePushedLayer = handlePushed
  target:handleSize(10) -- value doesn'T matter but must be set to something
end

function Pebble:buttonScrollbarDown(target, args)
  local b = lost.guiro.view.Button
  local l = lost.guiro.layer.Layer
  local r = lost.guiro.layer.Rect
  local i = lost.guiro.layer.Image
  local rr = lost.guiro.layer.RoundedRect
  local inset = self.scrollbarArrowInset[args.size]    
  target:mode("normal")
  local normal = l{sublayers={
                   r{filled=true,gradient="squareGlassMirrored",gradientVertical=false,bounds={0,0,"1","1"}},
                   i{flip=false,bounds={"center", "center", {"1",-inset}, {"1",-inset}},scale="aspect",color=self.scrollbarArrowColor, texture=self:varrowTexture(),filter=true,halign="center"},
                 }}
  local pushed = l{sublayers={
                   r{filled=true,gradient="candyBlueMirrored",gradientVertical=false,bounds={0,0,"1","1"}},
                   i{flip=false,bounds={"center", "center", {"1",-inset}, {"1",-inset}},scale="aspect",color=self.scrollbarArrowColorPushed, texture=self:varrowTexture(),filter=true,halign="center"},
                 }}

  target.layer:addSublayer(normal)
  target.layer:addSublayer(pushed)
  target.backgrounds[b.STATE_NORMAL] = normal
  target.backgrounds[b.STATE_PUSHED] = pushed
end

function Pebble:buttonScrollbarUp(target, args)
  local b = lost.guiro.view.Button
  local l = lost.guiro.layer.Layer
  local r = lost.guiro.layer.Rect
  local i = lost.guiro.layer.Image
  local rr = lost.guiro.layer.RoundedRect
  local inset = self.scrollbarArrowInset[args.size]  
  target:mode("normal")
  local normal = l{sublayers={
                   r{filled=true,gradient="squareGlassMirrored",gradientVertical=false,bounds={0,0,"1","1"}},
                   i{flip=true,bounds={"center", "center", {"1",-inset}, {"1",-inset}},scale="aspect",color=self.scrollbarArrowColor, texture=self:varrowTexture(),filter=true,halign="center"},
                 }}
  local pushed = l{sublayers={
                   r{filled=true,gradient="candyBlueMirrored",gradientVertical=false,bounds={0,0,"1","1"}},
                   i{flip=true,bounds={"center", "center", {"1",-inset}, {"1",-inset}},scale="aspect",color=self.scrollbarArrowColorPushed, texture=self:varrowTexture(),filter=true,halign="center"},
                 }}

  target.layer:addSublayer(normal)
  target.layer:addSublayer(pushed)
  target.backgrounds[b.STATE_NORMAL] = normal
  target.backgrounds[b.STATE_PUSHED] = pushed
end

function Pebble:buttonScrollbarLeft(target, args)
  local b = lost.guiro.view.Button
  local l = lost.guiro.layer.Layer
  local r = lost.guiro.layer.Rect
  local i = lost.guiro.layer.Image
  local rr = lost.guiro.layer.RoundedRect
  local inset = self.scrollbarArrowInset[args.size]
  target:mode("normal")
  local normal = l{sublayers={
                   r{filled=true,gradient="squareGlass",bounds={0,0,"1","1"}},
                   i{bounds={"center", "center", {"1",-inset}, {"1",-inset}},scale="aspect",color=self.scrollbarArrowColor, texture=self:larrowTexture(),filter=true,halign="center"},
                 }}
  local pushed = l{sublayers={
                  r{filled=true,gradient="candyBlue",bounds={0,0,"1","1"}},
                  i{bounds={"center", "center", {"1",-inset}, {"1",-inset}},scale="aspect",color=self.scrollbarArrowColorPushed, texture=self:larrowTexture(),filter=true,halign="center"},
                }}
  
  target.layer:addSublayer(normal)
  target.layer:addSublayer(pushed)
  target.backgrounds[b.STATE_NORMAL] = normal
  target.backgrounds[b.STATE_PUSHED] = pushed
end

function Pebble:buttonScrollbarRight(target, args)
  local b = lost.guiro.view.Button
  local l = lost.guiro.layer.Layer
  local r = lost.guiro.layer.Rect
  local i = lost.guiro.layer.Image
  local rr = lost.guiro.layer.RoundedRect
  local inset = self.scrollbarArrowInset[args.size]
  target:mode("normal")
  local normal = l{sublayers={
                   r{filled=true,gradient="squareGlass",bounds={0,0,"1","1"}},
                   i{bounds={"center", "center", {"1",-inset}, {"1",-inset}},scale="aspect",color=self.scrollbarArrowColor, texture=self:rarrowTexture(),filter=true,halign="center"},
                 }}
  local pushed = l{sublayers={
                 r{filled=true,gradient="candyBlue",bounds={0,0,"1","1"}},
                 i{bounds={"center", "center", {"1",-inset}, {"1",-inset}},scale="aspect",color=self.scrollbarArrowColorPushed, texture=self:rarrowTexture(),filter=true,halign="center"},
               }}
    
  target.layer:addSublayer(normal)
  target.layer:addSublayer(pushed)
  target.backgrounds[b.STATE_NORMAL] = normal
  target.backgrounds[b.STATE_PUSHED] = pushed
end

function Pebble:scrollBar(target, args)
  local sliderArgs = {}
  local sz = args.size or "regular"
  if args.mode == "horizontal" then
    target:height(self.scrollbarWidth[sz])
  else
    target:width(self.scrollbarWidth[sz])
  end
  local buttonSize = self.scrollbarButtonSize[sz]
  local sliderDelta = buttonSize*2
  sliderArgs.size = sz
  sliderArgs.mode = args.mode
  sliderArgs.style = "scrollbar"
  
  local slider = lost.guiro.view.Slider(sliderArgs)
  target:addSubview(slider)
  local sliderBounds = 0
  if args.mode == "horizontal" then 
    sliderBounds = lost.guiro.Bounds(0,0,{"1",-sliderDelta},"1")
  else
    sliderBounds = lost.guiro.Bounds(0,sliderDelta,"1",{"1",-sliderDelta})
  end
  slider:bounds(sliderBounds)
  target.slider = slider
  slider:handleSize(30)
  slider.scrollbarMode = true
  
  local buttonTheme = "pebble"
  local ibs = ""
  local dbs = ""
  local ibb = 0
  local dbb = 0
  if args.mode == "horizontal" then
    ibb = {"right",0,buttonSize,"1"}
    dbb = {{"right",-buttonSize},0,buttonSize,"1"}
    ibs = "scrollbarRight"
    dbs = "scrollbarLeft"
  else
    ibb = {0,buttonSize,"1",buttonSize}
    dbb = {0,0,"1",buttonSize}
    ibs = "scrollbarUp"
    dbs = "scrollbarDown"
  end
  local incButton = lost.guiro.view.Button{theme=buttonTheme, style=ibs,bounds=ibb,size=sz}
  local decButton = lost.guiro.view.Button{theme=buttonTheme, style=dbs,bounds=dbb,size=sz}
  target:addSubview(incButton)
  target:addSubview(decButton)
  target.incButton = incButton
  target.decButton = decButton
  target.buttonSize = buttonSize
  target:visibleRange(0.5)
  target:stepSize(0.1)
end

function Pebble:directoryView(target, args)

  target.directoryTexture = self:directoryTexture()
  target.fileTexture = self:fileTexture()

  target.entryHeight = 16
  target.entryPadding = 5

  target.createRow = function(self)
    return lost.guiro.view.View
    {
      bounds = {"left", "top", "1", target.entryHeight},
      sublayers =
      {
        lost.guiro.layer.Rect
        {
          id = "selection",
          bounds = {0, 0, "1", "1"},
          hidden = true,
          filled = true,
          color = Color(.4,.6,1,.2)
        },
        lost.guiro.layer.Image
        {
          id = "image",
          bounds = {{"left", target.entryPadding}, "center", 16, "1"},
          filter = true,
          texture = target.directoryTexture
        },
        lost.guiro.layer.Text
        {
          id = "text",
          bounds = {"right", "center", {"1", -(16+target.entryPadding*2)}, "1"},
          font = {"Vera", 10},
          halign = "left",
          valign = "top"
        }
      }
    }
  end

  target.layer:addSublayer(lost.guiro.layer.Rect{filled=true,color=Color(1,1,1),bounds={0,0,"1","1"}})
end


function Pebble:imageRoundCell(target, args)
  local l = lost.guiro.layer.Layer
  local rr = lost.guiro.layer.RoundedRect
  local i = lost.guiro.layer.Image
  
  local m=0
  local d=1
  local t = m+d
  local rad=4
  local bgcol = Color(0,0,0,.3)
  local framecol = Color(1,1,1)

  local bg = l{
    sublayers=
    {
      rr{bounds={m,m,{"1",-2*m},{"1",-2*m}},filled=true,radius=rad,color=bgcol},
      rr{bounds={t,t,{"1",-2*t},{"1",-2*t}},filled=false,width=1,radius=rad,color=framecol},
    }
  }
  
  local imglayer = i{bounds={0,0,"1","1"},scale=none,valign="center",halign="center",flip=true,filter=false}
  
  target.layer:addSublayer(bg)
  target.layer:addSublayer(imglayer)
  target.imageLayer = imglayer
end

function Pebble:scrollView(target, args)
  target:clip(true)
  local sz = args.size or "regular"
  local sbwidth = self.scrollbarWidth[sz]
  local vsb = lost.guiro.view.ScrollBar{bounds={"right",sbwidth,sbwidth,{"1",-sbwidth}},size=sz,mode="vertical"}
  local hsb = lost.guiro.view.ScrollBar{bounds={0,0,{"1",-sbwidth},sbwidth},size=sz,mode="horizontal"}
  local contentView = lost.guiro.view.View{}
  local corner = lost.guiro.view.View{bounds={"right",0,sbwidth,sbwidth},sublayers={lost.guiro.layer.Rect{bounds={0,0,sbwidth,sbwidth},filled=true,color=Color(250/255,250/255,250/255)}}}
  target:addSubview(contentView, true)
  target:addSubview(vsb, true)
  target:addSubview(hsb, true)
  target:addSubview(corner,true)
  target.verticalScrollbar = vsb
  target.horizontalScrollbar = hsb
  target.scrollbarWidth = sbwidth
  target.contentView = contentView
  target.cornerView = corner
end

function Pebble:listView(target, args)
  self:scrollView(target, args)
end
  
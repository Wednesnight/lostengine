require("lost.guiro.view.Window")

local Color = lost.common.Color

local titleHeight = 30

local window1 = require("window1")
local window2 = require("window2")
local windowControl = require("windowControl")

return lost.guiro.view.View
{
  bounds = {0,0,"1","1"},
  id="windowContainer",
  sublayers=
  {
    lost.guiro.layer.Rect{bounds={0,0,"1","1"},color=Color(0,0,0),filled=true}
  },
  listeners=
  {
   windowClose = function(event)
    log.debug(event.target.id)
    if event.target.id == "window1" then windowControl("content")("wb1"):pushed(false) end
    if event.target.id == "window2" then windowControl("content")("wb2"):pushed(false) end
   end,
   buttonClick = function(event) 
    log.debug(event.target.id)
    if event.target.id == "wb1" then window1:hidden(false) end
    if event.target.id == "wb2" then window2:hidden(false) end
   end,
  },
  subviews=
  {
    lost.guiro.view.Label
    {
      bounds = {0,"top","1",titleHeight},
      text = "Window",
      font = {"Vera",12},
      color = Color(1,1,1)
    },
    window1,window2,windowControl
  },
}
local window1 = require("window1")
local window2 = require("window2")
local windowControl = require("windowControl")

return lost.guiro.view.View
{
  bounds = {0,0,"1","1"},
  id="windowContainer",
  listeners=
  {
   windowClose = function(event)
    if event.target.id == "window1" then windowControl("content")("wb1"):pushed(false) end
    if event.target.id == "window2" then windowControl("content")("wb2"):pushed(false) end
   end,
   buttonClick = function(event) 
    if event.target.id == "wb1" then window1:hidden(false) end
    if event.target.id == "wb2" then window2:hidden(false) end
   end,
  },
  subviews=
  {
    window1,window2,windowControl
  },
}
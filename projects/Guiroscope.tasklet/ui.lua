local Bounds = lost.guiro.Bounds
local xabs = lost.guiro.xabs
local yabs = lost.guiro.yabs
local xrel = lost.guiro.xrel
local yrel = lost.guiro.yrel
local xright = lost.guiro.xright
local xleft = lost.guiro.xleft
local ytop = lost.guiro.ytop
local ybottom = lost.guiro.ybottom
local xcenter = lost.guiro.xcenter
local ycenter = lost.guiro.ycenter
local wabs = lost.guiro.wabs
local habs = lost.guiro.habs
local wrel = lost.guiro.wrel
local hrel = lost.guiro.hrel
local Color = lost.common.Color
local Rect = lost.math.Rect
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3

return dcl.guiro:Screen
{
  id = "screen",
  listeners = 
  {
--    mouseEnter = function(event) log.debug("enter "..event.target.id.." "..tostring(event.target:globalRect())) end,
--    mouseLeave = function(event) log.debug("leave "..event.target.id) end,
--    mouseDown = function(event) log.debug("down "..event.target.id) end,
--    mouseUp = function(event) log.debug("up "..event.target.id) end,
--    mouseUpInside = function(event) log.debug("up inside "..event.target.id) end,
--    mouseUpOutside = function(event) log.debug("up outside "..event.target.id) end,
--    buttonClick = function(event) log.debug("CLICKED "..event.target.id) end,
--    mouseScroll = function(event) log.debug("scroll ".. event.target.id .." ".. tostring(event.scrollDelta)) end
  },
  dcl.guiro:UserInterface
  {
    id = "mainUi",
    bounds = Bounds(xabs(0), yabs(0), wrel(1), hrel(1)),
    dcl.guiro:Window
    {
      id = "window1",
      bounds = Bounds(xabs(0), yabs(0), wrel(.5), hrel(.5)),
      dcl.guiro:ScrollView
      {
        bounds = Bounds(xabs(0), yabs(0), wrel(1), hrel(1)),
        contentViewBounds = Bounds(xleft(), ytop(), wabs(550), habs(265)),
        dcl.guiro:Image
        {
          id = "image1",
          bounds = Bounds(xabs(10), yabs(100), wabs(100), habs(75)),
          filename = "lost/guiro/themes/default/resources/ButtonHover.png",
          scale="scalegrid",
          showFrame = false,
          caps = 
          {
            left = 3,
            right=3,
            top=3,
            bottom=3
          },
          flip = true,
          filter = true,
        },
        dcl.guiro:Button
        {
          bounds = Bounds(xabs(50), yabs(240), wabs(500), habs(25)),
          title = "riesig",
          listeners = 
          {
            mouseEnter = function(event) event.target:title("RIESIG") end,
            mouseLeave = function(event) event.target:title("riesig") end
          }
        }
      }
    },
    dcl.guiro:Window
    {
      id = "window2",
      bounds = Bounds(xright(), ytop(), wrel(.5), hrel(1)),
			dcl.guiro:HBox
			{
				bounds = Bounds(xleft(), ytop(), wrel(1), habs(300)),
				halign = "center",
				valign = "center",
				mode = "spread",
  			dcl.guiro:VBox
  			{
  				bounds = Bounds(xleft(), ybottom(), wabs(100), habs(270)),
  				halign = "center",
  				valign = "center",
  				mode = "stack",
  	      dcl.guiro:Label
  	      {
  	        id="label1",
  	        bounds = Bounds(xcenter(), ycenter(), wabs(100), habs(30)),
  	        text = "Hello!",
  	        listeners = 
  	        {
  	          mouseEnter = function(event)
															event.target:showFrame(true)
															event.target:screen()("mainUi")("window1"):hidden(true)
													 end,
  	          mouseLeave = function(event)
													 	 event.target:showFrame(false)
															event.target:screen()("mainUi")("window1"):hidden(false)
												   end
  	        }          
  	      },
  				dcl.guiro:Label
  				{
  	        bounds = Bounds(xabs(0), yabs(0), wabs(100), habs(40)),					
  					text="noch eins"
  				},
  				dcl.guiro:Label
  				{
  	        bounds = Bounds(xabs(0), yabs(0), wabs(100), habs(30)),					
  					text="und noch eins"
  				},
  				dcl.guiro:Label
  				{
  	        bounds = Bounds(xabs(0), yabs(0), wabs(100), habs(40)),					
  					text="noch eins"
  				},
  				dcl.guiro:Label
  				{
  	        bounds = Bounds(xabs(0), yabs(0), wabs(100), habs(30)),					
  					text="und noch eins"
  				}
  			},
  			dcl.guiro:VBox
  			{
  				bounds = Bounds(xleft(), ybottom(), wabs(100), habs(270)),
  				halign = "center",
  				valign = "center",
  				mode = "stack",
  	      dcl.guiro:Label
  	      {
  	        id="label1",
  	        bounds = Bounds(xcenter(), ycenter(), wabs(100), habs(30)),
  	        text = "Hello!",
  	        listeners = 
  	        {
  	          mouseEnter = function(event) event.target:showFrame(true) end,
  	          mouseLeave = function(event) event.target:showFrame(false) end
  	        }          
  	      },
  				dcl.guiro:Label
  				{
  	        bounds = Bounds(xabs(0), yabs(0), wabs(100), habs(40)),					
  					text="noch eins"
  				},
  				dcl.guiro:Label
  				{
  	        bounds = Bounds(xabs(0), yabs(0), wabs(100), habs(30)),					
  					text="und noch eins"
  				},
  				dcl.guiro:Label
  				{
  	        bounds = Bounds(xabs(0), yabs(0), wabs(100), habs(40)),					
  					text="noch eins"
  				},
  				dcl.guiro:Label
  				{
  	        bounds = Bounds(xabs(0), yabs(0), wabs(100), habs(30)),					
  					text="und noch eins"
  				}
  			},
  			dcl.guiro:VBox
  			{
  				bounds = Bounds(xleft(), ybottom(), wabs(100), habs(270)),
  				halign = "center",
  				valign = "center",
  				mode = "stack",
  	      dcl.guiro:Label
  	      {
  	        id="label1",
  	        bounds = Bounds(xcenter(), ycenter(), wabs(100), habs(30)),
  	        text = "Hello!",
  	        listeners = 
  	        {
  	          mouseEnter = function(event) event.target:showFrame(true) end,
  	          mouseLeave = function(event) event.target:showFrame(false) end
  	        }          
  	      },
  				dcl.guiro:Label
  				{
  	        bounds = Bounds(xabs(0), yabs(0), wabs(100), habs(40)),					
  					text="noch eins"
  				},
  				dcl.guiro:Label
  				{
  	        bounds = Bounds(xabs(0), yabs(0), wabs(100), habs(30)),					
  					text="und noch eins"
  				},
  				dcl.guiro:Label
  				{
  	        bounds = Bounds(xabs(0), yabs(0), wabs(100), habs(40)),					
  					text="noch eins"
  				},
  				dcl.guiro:Label
  				{
  	        bounds = Bounds(xabs(0), yabs(0), wabs(100), habs(30)),					
  					text="und noch eins"
  				}
  			}				
			}
    }
    
  }
}

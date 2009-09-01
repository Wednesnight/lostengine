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
    mouseEnter = function(event) log.debug("enter "..event.target.id.." "..tostring(event.target:globalRect())) end,
    mouseLeave = function(event) log.debug("leave "..event.target.id) end
  },
  dcl.guiro:UserInterface
  {
    id = "mainUi",
    bounds = Bounds(xabs(0), yabs(0), wrel(1), hrel(1)),
    dcl.guiro:Window
    {
      id = "window1",
      bounds = Bounds(xabs(0), yabs(0), wrel(.5), hrel(.5)),
      dcl.guiro:Image
      {
        id = "image1",
        bounds = Bounds(xabs(10), yabs(10), wabs(150), habs(75)),
        filename = "stubs.jpg",
      }
    },
    dcl.guiro:Window
    {
      id = "window2",
      bounds = Bounds(xright(), ytop(), wrel(.5), hrel(1)),
			dcl.guiro:HBox
			{
				bounds = Bounds(xleft(), ytop(), wrel(1), habs(100)),
				halign = "center",
				valign = "center",
				mode = "stack",
				backgroundColor = Color(1,0,0),
				showBackground = true,
				showFrame = true,
				dcl.guiro:Label
				{
					id="hlabel1",
					showFrame = true,
	        bounds = Bounds(xabs(0), yabs(0), wabs(100), habs(40)),					
					text="noch eins"
				},
				dcl.guiro:Label
				{
					id="hlabel2",
					showFrame = true,
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
				showFrame = true,
	      dcl.guiro:Label
	      {
	        id="label1",
	        bounds = Bounds(xcenter(), ycenter(), wabs(100), habs(30)),
	        text = "Chello!",
	        listeners = 
	        {
	          mouseEnter = function(event) event.target:showFrame(true) end,
	          mouseLeave = function(event) event.target:showFrame(false) end
	        }          
	      },
				dcl.guiro:Label
				{
					showFrame = true,
	        bounds = Bounds(xabs(0), yabs(0), wabs(100), habs(40)),					
					text="noch eins"
				},
				dcl.guiro:Label
				{
					showFrame = true,
	        bounds = Bounds(xabs(0), yabs(0), wabs(100), habs(30)),					
					text="und noch eins"
				},
				dcl.guiro:Label
				{
					showFrame = true,
	        bounds = Bounds(xabs(0), yabs(0), wabs(100), habs(40)),					
					text="noch eins"
				},
				dcl.guiro:Label
				{
					showFrame = true,
	        bounds = Bounds(xabs(0), yabs(0), wabs(100), habs(30)),					
					text="und noch eins"
				}
			}
    }
  }
}


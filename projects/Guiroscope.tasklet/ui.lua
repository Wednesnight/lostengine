using "lost.common.Color"
using "lost.math.Rect"
using "lost.math.Vec2"
using "lost.math.Vec3"

return dcl.guiro:UserInterface
{
  id = "ui",
  listeners = 
  {
    mouseEnter = function(event) log.debug("enter "..event.target.id.." "..tostring(event.target.rect)) end,
    mouseLeave = function(event) log.debug("leave "..event.target.id) end,
    mouseDown = function(event) log.debug("down "..event.target.id) end,
    mouseUp = function(event) log.debug("up "..event.target.id) end,
    mouseUpInside = function(event) log.debug("up inside "..event.target.id) end,
    mouseUpOutside = function(event) log.debug("up outside "..event.target.id) end,
    buttonClick = function(event) log.debug("CLICKED "..event.target.id) end,
    mouseScroll = function(event) log.debug("scroll ".. event.target.id .." ".. tostring(event.scrollDelta)) end
  },
  dcl.guiro:View
  {
    id = "view1",
    bounds = {0,0,".5", ".5"},
    dcl.guiro:Image
    {
      id = "image1",
      bounds = {10,100,100,75},
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
      bounds = {50,240,500,25},
      title = "riesig",
      listeners = 
      {
        mouseEnter = function(event) if event.currentTarget == event.target then event.target:title("RIESIG") end end,
        mouseLeave = function(event) if event.currentTarget == event.target then event.target:title("riesig") end end
      }
    }
  },
  dcl.guiro:View
  {
    id = "view2",
    bounds = {"right", "top", ".5", "1"},
		dcl.guiro:HBox
		{
			bounds = {"left", "top", "1", 300},
			halign = "center",
			valign = "center",
			mode = "spread",
			dcl.guiro:VBox
			{
				bounds = {"left", "bottom", 100, 270},
				halign = "center",
				valign = "center",
				mode = "stack",
	      dcl.guiro:Label
	      {
	        id="label1",
	        bounds = {"center", "center", 100, 30},
	        text = "Hello!",
	        listeners = 
	        {
	          mouseEnter = function(event)
	                          if event.currentTarget == event.target then
															event.target:showFrame(true)
															event.target:rootView()("mainUi")("view1"):hidden(true)
														end
												 end,
	          mouseLeave = function(event)
                            if event.currentTarget == event.target then
												 	    event.target:showFrame(false)
														  event.target:rootView()("mainUi")("view1"):hidden(false)
														end
											   end
	        }          
	      },
				dcl.guiro:Label
				{
	        bounds = {0,0,100,40},
					text="noch eins"
				},
				dcl.guiro:Label
				{
	        bounds = {0,0,100,30},
					text="und noch eins"
				},
				dcl.guiro:Label
				{
	        bounds = {0,0,100,40},
					text="noch eins"
				},
				dcl.guiro:Label
				{
	        bounds = {0,0,100,30},
					text="und noch eins"
				}
			},
			dcl.guiro:VBox
			{
				bounds = {"left", "bottom", 100, 270},
				halign = "center",
				valign = "center",
				mode = "stack",
	      dcl.guiro:Label
	      {
	        id="label1",
	        bounds = {"center", "center", 100, 30},
	        text = "Hello!",
	        listeners = 
	        {
	          mouseEnter = function(event) if event.currentTarget == event.target then event.target:showFrame(true) end end,
	          mouseLeave = function(event) if event.currentTarget == event.target then event.target:showFrame(false) end end
	        }          
	      },
				dcl.guiro:Label
				{
	        bounds = {0,0,100,40},
					text="noch eins"
				},
				dcl.guiro:Label
				{
	        bounds = {0,0,100,30},
					text="und noch eins"
				},
				dcl.guiro:Label
				{
	        bounds = {0,0,100,40},
					text="noch eins"
				},
				dcl.guiro:Label
				{
	        bounds = {0,0,100,30},
					text="und noch eins"
				}
			},
			dcl.guiro:VBox
			{
				bounds = {"left", "bottom", 100, 270},
				halign = "center",
				valign = "center",
				mode = "stack",
	      dcl.guiro:Label
	      {
	        id="label1",
	        bounds = {"center", "center", 100, 30},
	        text = "Hello!",
	        listeners = 
	        {
	          mouseEnter = function(event) if event.currentTarget == event.target then event.target:showFrame(true) end end,
	          mouseLeave = function(event) if event.currentTarget == event.target then event.target:showFrame(false) end end
	        }          
	      },
				dcl.guiro:Label
				{
	        bounds = {0,0,100,40},
					text="noch eins"
				},
				dcl.guiro:Label
				{
	        bounds = {0,0,100,30},
					text="und noch eins"
				},
				dcl.guiro:Label
				{
	        bounds = {0,0,100,40},
					text="noch eins"
				},
				dcl.guiro:Label
				{
	        bounds = {0,0,100,30},
					text="und noch eins"
				}
			}				
		}    
  }
}


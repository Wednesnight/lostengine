require("lost.declarative.Context")

using "lost.math.Vec2"
using "lost.math.Rect"
using "lost.application.WindowParams"
using "lost.guiro.Bounds"
using "lost.common.Color"
using "lost.guiro.wrel"
using "lost.guiro.hrel"
using "lost.guiro.xabs"
using "lost.guiro.yabs"
using "lost.guiro.xleft"
using "lost.guiro.ybottom"
using "lost.guiro.habs"
using "lost.guiro.wabs"
using "lost.guiro.xleft"
using "lost.guiro.ybottom"
using "lost.guiro.ycenter"

screensize = Vec2(640,480)
windowParams = WindowParams("rrtest", Rect(50,50,screensize.x, screensize.y))

running = true
rootNode = nil
dcl = nil
screen = nil

function startup(tasklet)
  tasklet.name = "rrtest"
  tasklet.waitForEvents = true
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)  
  dcl = lost.declarative.Context(tasklet.loader)
  
  -- add clear commands before all others
  tasklet.renderNode:add(lost.rg.ClearColor.create(lost.common.Color(0,0,0,1)))
  tasklet.renderNode:add(lost.rg.Clear.create(gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT))
  
  -- guiro.Screen adds itself implicitly to the tasklets renderNode, so call it last
  screen = dcl.guiro:Screen
  {
    dcl.guiro:UserInterface
    {
      id = "ui",
      bounds = Bounds(xleft(), ybottom(), wrel(1), hrel(1)),
      dcl.guiro:Window
      {
        id = "window",
        bounds = Bounds(xleft(), ybottom(), wrel(1), hrel(1)),
        showBackground = true,
				showFrame = false,
        backgroundColor = Color(.9294,.9294,.9294),
--        frameColor = Color(1,1,1),
        dcl.guiro:HBox
        {
          showFrame = true,
          frameColor = Color(0,1,0),
--          showBackground = true,
--          backgroundColor = Color(0,1,0),
          bounds = Bounds(xleft(),ybottom(), wrel(1), hrel(1)),
          mode = "stack",
  				dcl.guiro:VBox
  				{
  				  showFrame = true,
  				  frameColor = Color(1,0,0),
  					bounds = Bounds(xabs(0), yabs(0), wabs(200), hrel(1)),
  					mode = "stack",
  					spacing = 10,
  					valign = "center",
  	        dcl.guiro:Button
  	        {
  	          id = "roundedRectButton",
  	          theme = "pebble",
  	          style = "RoundedRect",
  	          bounds = Bounds(xabs(50), yabs(50), wabs(100), habs(18)), 
  	          title = "Timmy !!!!"       
  	        },
  	        dcl.guiro:Button
  	        {
  	          id = "roundedRectButton",
  	          theme = "pebble",
  	          style = "RoundedRect",
  	          bounds = Bounds(xabs(50), yabs(50), wabs(150), habs(18)), 
  	          title = "Rounded Rect Button"       
  	        }
  				},
  				dcl.guiro:VBox
  				{
  					bounds = Bounds(xabs(0), ycenter(), wabs(200), hrel(1)),
  					mode = "stack",
  					spacing = 10,
  					valign = "center",
  					showFrame = true,
  					frameColor = Color(0,0,1),
  	        dcl.guiro:Button
  	        {
  	          id = "roundedRectButton",
  	          theme = "pebble",
  	          style = "RoundedRect",
  	          bounds = Bounds(xabs(50), yabs(50), wabs(100), habs(18)), 
  	          title = "Timmy !!!!"       
  	        },
  	        dcl.guiro:Button
  	        {
  	          id = "roundedRectButton",
  	          theme = "pebble",
  	          style = "RoundedRect",
  	          bounds = Bounds(xabs(50), yabs(50), wabs(150), habs(18)), 
  	          title = "Rounded Rect Button"       
  	        }
  				}
        }
      }
    }
  }
  
  return true
end

function update(tasklet)
  return running
end

function keyHandler(event)
  if (event.key == lost.application.K_ESCAPE) then
    running = false
  end
end

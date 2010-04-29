require("lost.declarative.Context")
require("lost.common.Shaders")

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
using "lost.guiro.xcenter"
using "lost.guiro.ybottom"
using "lost.guiro.ycenter"

screensize = Vec2(320,480)
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
--        backgroundColor = Color(0,0,0,0),
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
  					spacing = 2,
  					valign = "center",
  	        dcl.guiro:Button
  	        {
  	          id = "roundedRectButton",
  	          theme = "pebble",
  	          style = "RoundedRectRegular",
  	          bounds = Bounds(xabs(50), yabs(50), wabs(80), nil), 
  	          title = "Regular"       
  	        },
  	        dcl.guiro:Button
  	        {
  	          id = "roundedRectButton",
  	          theme = "pebble",
  	          style = "RoundedRectSmall",
  	          bounds = Bounds(xabs(50), yabs(50), wabs(80), nil), 
  	          title = "Small"       
  	        },
  	        dcl.guiro:Button
  	        {
  	          id = "roundedRectButton",
  	          theme = "pebble",
  	          style = "RoundedRectMini",
  	          bounds = Bounds(xabs(50), yabs(50), wabs(80), nil), 
  	          title = "Mini"       
  	        },
  	        dcl.guiro:HBox
  	        {
  	          bounds=Bounds(xcenter(), nil, wrel(1), habs(20)),
  	          mode="stack",
  	          spacing=2,
  	          valign = "center",
              dcl.guiro:View
    	        {
    	            theme = "pebble",
    	            style="bubbleGrey",
    	            size="regular",
    	        },
              dcl.guiro:View
    	        {
    	            theme = "pebble",
    	            style="bubbleGrey",
    	            size="small",
    	        },
              dcl.guiro:View
    	        {
    	            theme = "pebble",
    	            style="bubbleGrey",
    	            size="mini",
    	        },
              dcl.guiro:View
    	        {
    	            theme = "pebble",
    	            style="bubbleGrey",
    	        }
    	      },
  	        dcl.guiro:HBox
  	        {
  	          bounds=Bounds(xcenter(), nil, wrel(1), habs(20)),
  	          mode="stack",
  	          spacing=2,
  	          valign = "center",
              dcl.guiro:View
    	        {
    	            theme = "pebble",
    	            style="bubbleBlue",
    	            size="regular",
    	        },
              dcl.guiro:View
    	        {
    	            theme = "pebble",
    	            style="bubbleBlue",
    	            size="small",
    	        },
              dcl.guiro:View
    	        {
    	            theme = "pebble",
    	            style="bubbleBlue",
    	            size="mini",
    	        },
              dcl.guiro:View
    	        {
    	            theme = "pebble",
    	            style="bubbleBlue",
    	        }
    	      },
    	      dcl.guiro:HBox
    	      {
    	        bounds = Bounds(xleft(), nil, wrel(1), habs(30)),
    	        mode="stack",
    	        spacing=0,
    	        valign="center",
              dcl.guiro:View
    	        {
    	            bounds = Bounds(nil, yabs(20), wabs(50), nil),
    	            theme = "pebble",
    	            style = "segment",
    	            size = "regular",
    	            segmentColor = "blue",
    	            orientation = "left"
    	        },
              dcl.guiro:View
    	        {
    	            bounds = Bounds(nil, yabs(20), wabs(40), nil),
    	            theme = "pebble",
    	            style = "segment",
    	            size = "regular",
    	            segmentColor = "grey",
    	            orientation = "mid"
    	        },
              dcl.guiro:View
    	        {
    	            bounds = Bounds(nil, yabs(20), wabs(30), nil),
    	            theme = "pebble",
    	            style = "segment",
    	            size = "regular",
    	            segmentColor = "grey",
    	            orientation = "right"
    	        }
    	      },
    	      dcl.guiro:HBox
    	      {
    	        bounds = Bounds(xleft(), nil, wrel(1), habs(30)),
    	        mode="stack",
    	        spacing=0,
    	        valign="center",
              dcl.guiro:View
    	        {
    	            bounds = Bounds(nil, yabs(20), wabs(30), nil),
    	            theme = "pebble",
    	            style = "segment",
    	            size = "mini",
    	            segmentColor = "blue",
    	            orientation = "left"
    	        },
              dcl.guiro:View
    	        {
    	            bounds = Bounds(nil, yabs(20), wabs(30), nil),
    	            theme = "pebble",
    	            style = "segment",
    	            size = "mini",
    	            segmentColor = "grey",
    	            orientation = "mid"
    	        },
              dcl.guiro:View
    	        {
    	            bounds = Bounds(nil, yabs(20), wabs(30), nil),
    	            theme = "pebble",
    	            style = "segment",
    	            size = "mini",
    	            segmentColor = "grey",
    	            orientation = "right"
    	        }
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
  	          bounds = Bounds(xabs(50), yabs(50), wabs(100), nil), 
  	          title = "Base SDK"       
  	        },
  	        dcl.guiro:Button
  	        {
  	          id = "roundedRectButton",
  	          theme = "pebble",
  	          style = "RoundedRect",
  	          bounds = Bounds(xabs(50), yabs(50), wabs(150), nil), 
  	          title = "Base SDK | Debug"       
  	        }
  				}
        }
      }
    }
  }
  
  tasklet.renderNode:add(
    dcl.rg:Node
    {
      dcl.rg:ClearColor
      {
        color = Color(0,0,0,0)
      },
      dcl.rg:Clear
      {
        mask = gl.GL_COLOR_BUFFER_BIT+gl.GL_DEPTH_BUFFER_BIT
      },
      dcl.rg:DepthTest{false},
      dcl.rg:Camera2D
      {
        viewport = Rect(0,0,screensize.x, screensize.y)
      },    
      dcl.rg:Draw
      {
        mesh = dcl.mesh:Quad
        {
          texture = dcl.guiro.textureManager._textureManager.gradientTexture,
          material = 
          {
            blend = true,
            color = Color(1,1,1),
            shader = lost.common.Shaders.textureShader()
          }
        }
      }
    }
  )
  
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

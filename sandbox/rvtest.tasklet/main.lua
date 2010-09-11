require("lost.declarative.Context")
require("lost.common.Shaders")

using "lost.math.Vec2"
using "lost.math.Rect"
using "lost.application.WindowParams"
using "lost.common.Color"

config = require("config")
running = true
rootNode = nil
dcl = nil
screen = nil

function startup()
  tasklet.waitForEvents = true
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)  
  dcl = lost.declarative.Context(tasklet.loader)
  
  -- add clear commands before all others
  tasklet.renderNode:add(lost.rg.ClearColor.create(lost.common.Color(0,0,0,1)))
  tasklet.renderNode:add(lost.rg.Clear.create(gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT))
  
  
  -- guiro.Screen adds itself implicitly to the tasklets renderNode, so call it last
  screen = dcl.guiro:Screen
  {
    listeners = 
    {
      buttonClick = function(event)
                      log.debug("clicked on '"..tostring(event.target.id).."'")
                      if event.target.id == "roundedRectButtonRegular" then
                        screen:printSubviews()
                        screen.rootNode:print()                        
                      end
                    end,
    },
    dcl.guiro:UserInterface
    {
      id = "ui",
      bounds = {"left", "bottom", "1", "1"},
      dcl.guiro:Window
      {
        id = "window",
        bounds = {"left", "bottom", "1", "1"},
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
          bounds = {"left", "bottom", "1", "1"},
          mode = "stack",
  				dcl.guiro:VBox
  				{
  				  showFrame = true,
  				  frameColor = Color(1,0,0),
  					bounds = {0,0,".25", "1"},
  					mode = "stack",
  					spacing = 2,
  					valign = "center",
  	        dcl.guiro:Button
  	        {
  	          id = "roundedRectButtonRegular",
  	          theme = "pebble",
  	          style = "RoundedRectRegular",
  	          bounds = {50,50,80,nil}, 
  	          title = "Regular",
  	          mode = "normal",
  	        },
  	        dcl.guiro:Button
  	        {
  	          id = "roundedRectButtonSmall",
  	          theme = "pebble",
  	          style = "RoundedRectSmall",
  	          bounds = {50,50,60,nil}, 
  	          title = "Small",       
  	          mode = "sticky"      
  	        },
  	        dcl.guiro:Button
  	        {
  	          id = "roundedRectButtonMini",
  	          theme = "pebble",
  	          style = "RoundedRectMini",
  	          bounds = {50,50,40,nil}, 
  	          title = "Mini",       
  	          mode = "toggle"      
  	        },
  	        dcl.guiro:HBox
  	        {
  	          bounds={"center", nil, "1", 20},
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
  	          bounds={"center", nil, "1", 20},
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
    	        bounds = {"left", nil, "1", 30},
    	        mode="stack",
    	        spacing=0,
    	        valign="center",
              dcl.guiro:View
    	        {
    	            bounds = {nil, 20, ".3", nil},
    	            theme = "pebble",
    	            style = "segment",
    	            size = "regular",
    	            segmentColor = "blue",
    	            orientation = "left"
    	        },
              dcl.guiro:View
    	        {
									hidden = false,
    	            bounds = {nil, 20, ".3", nil},
    	            theme = "pebble",
    	            style = "segment",
    	            size = "regular",
    	            segmentColor = "grey",
    	            orientation = "mid"
    	        },
              dcl.guiro:View
    	        {
  								hidden = false,
    	            bounds = {nil, 20, ".3", nil},
    	            theme = "pebble",
    	            style = "segment",
    	            size = "regular",
    	            segmentColor = "grey",
    	            orientation = "right"
    	        }
    	      },
    	      dcl.guiro:HBox
    	      {
    	        bounds = {"left", nil, "1", 30},
    	        mode="stack",
    	        spacing=0,
    	        valign="center",
              dcl.guiro:View
    	        {
    	            bounds = {nil, 20, 30, nil},
    	            theme = "pebble",
    	            style = "segment",
    	            size = "mini",
    	            segmentColor = "blue",
    	            orientation = "left"
    	        },
              dcl.guiro:View
    	        {
    	            bounds = {nil, 20, 30, nil},
    	            theme = "pebble",
    	            style = "segment",
    	            size = "mini",
    	            segmentColor = "grey",
    	            orientation = "mid"
    	        },
              dcl.guiro:View
    	        {
    	            bounds = {nil, 20, 30, nil},
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
  					bounds = {0, "center", 200, "1"},
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
  	          bounds = {50,50,100,nil}, 
  	          title = "Base SDK"       
  	        },
  	        dcl.guiro:Button
  	        {
  	          id = "roundedRectButton",
  	          theme = "pebble",
  	          style = "RoundedRect",
  	          bounds = {50,50,150,nil}, 
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
        viewport = Rect(0,0,config.window.width, config.window.height)
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

function update()
  return running
end

function keyHandler(event)
  if (event.key == lost.application.K_ESCAPE) then
    running = false
  end
end

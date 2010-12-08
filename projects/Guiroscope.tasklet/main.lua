require("lost.declarative.Context")
require("lost.guiro.view.Label")
require("lost.guiro.layer.Text")
require("lost.guiro.layer.Rect")
require("lost.guiro.layout.Horizontal")
require("lost.guiro.layout.Vertical")

local Color = lost.common.Color

function startup()
  local tabbarheight = 52
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)
  ui = lost.guiro.view.UserInterface
  {
    listeners = 
    {
--      mouseEnter = function(event) log.debug("enter "..event.target.id.." "..tostring(event.target.rect)) end,
--      mouseLeave = function(event) log.debug("leave "..event.target.id) end,
--      mouseDown = function(event) log.debug("down "..event.target.id) end,
--      mouseUp = function(event) log.debug("up "..event.target.id) end,
--      mouseUpInside = function(event) log.debug("up inside "..event.target.id) end,
--      mouseUpOutside = function(event) log.debug("up outside "..event.target.id) end,
      buttonClick = function(event) log.debug("buttonClick "..event.target.id) end,
      buttonUp = function(event) log.debug("buttonUp "..event.target.id) end,
      buttonDown = function(event) log.debug("buttonDown "..event.target.id) end,
--      mouseScroll = function(event) log.debug("scroll ".. event.target.id .." ".. tostring(event.scrollDelta)) end
    },
    subviews = 
    {
      lost.guiro.view.View
      {
          style="toolbar", bounds={0,"top","1",tabbarheight},
          subviews=
          {
            lost.guiro.view.Label{bounds={10,0,"1",50},halign="left", text="Guiroscope",font={"Grinched",30}},
          }
      },
      lost.guiro.view.View
      {
        bounds = {0,0,"1",{"1",-tabbarheight}},
        layout = lost.guiro.layout.Vertical{halign="center",valign = "center",xoffset = -0,yoffset = 0,spacing = 10,},
        subviews=
        {
          lost.guiro.view.Label{bounds={200,200,100,30},text="hello",style="roundFramed"},
          lost.guiro.view.Button{id="ugly",mode="sticky",bounds={50,0,100,30},title="Button",},
          lost.guiro.view.Button{id="regular",mode="normal",bounds={50,{"top",-10},154,50},style="rounded",size="regular", title="Regular"},
          lost.guiro.view.Button{id="small",mode="sticky",bounds={50,{"top",-30},154,70},style="rounded",size="small", title="Small"},
          lost.guiro.view.Button{id="mini",bounds={50,{"top",-50},154,90},style="rounded",size="mini", title="Mini"},
          lost.guiro.view.Button{id="regular",mode="toggle",bounds={50,{"top",-10},154,50},style="roundedToggle",size="regular",title="Toggle"},
        }
      }
    }
  }
  ui:printSubviews()
end

function update()
--  ui.layer.renderNode:print()
end

function keyHandler(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end

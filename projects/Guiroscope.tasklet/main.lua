require("lost.declarative.Context")
require("lost.guiro.view.Label")
require("lost.guiro.layer.Text")
require("lost.guiro.layer.Rect")

local Color = lost.common.Color

function startup()
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)
  local ui = lost.guiro.ui()
  ui:add
  {
    lost.guiro.view.View
    {
      style="gray",
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
      subviews=
      {
        lost.guiro.view.Label
        {
          bounds = {200,200,100,30},
          text = "hello",
          style="roundFramed"
        },
        lost.guiro.view.Button
        {
          bounds = {50,0,100,30},
          text = "Button",
        },
        lost.guiro.view.Button
        {
          bounds = {50,70,100,50},
          text = "Rounded Regular",
          style = "rounded",
          size = "regular"
        },
        lost.guiro.view.Button
        {
          bounds = {50,90,100,70},
          text = "Rounded Small",
          style = "rounded",
          size = "small"
        },
        lost.guiro.view.Button
        {
          bounds = {50,110,100,90},
          text = "Rounded Mini",
          style = "rounded",
          size = "mini"
        }
      }
    }
  }
  ui:printSubviews()
end

function keyHandler(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end

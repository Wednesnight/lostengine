local Bounds = lost.guiro.Bounds
local xabs = lost.guiro.xabs
local yabs = lost.guiro.yabs
local xrel = lost.guiro.xrel
local yrel = lost.guiro.yrel
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
    mouseEnter = function(event) log.debug("enter "..event.target.id) end,
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
      showFrame = false,
      frameColor = Color(1,1,1),
      showBackground = true,
      backgroundColor = Color(1,1,0)
    },
    dcl.guiro:Window
    {
      id = "window2",
      bounds = Bounds(xrel(.5), yrel(.5), wrel(.5), hrel(.5)),
      showFrame = true,
      frameColor = Color(0,1,0),
      showBackground = true,
      backgroundColor = Color(1,0,0),
      dcl.guiro:Label
      {
        id="label1",
        bounds = Bounds(xcenter(), ycenter(), wabs(100), habs(30)),
        showFrame=false,
        text="Hello World",
        font = dcl.font:TrueTypeFont
        {
          filename = "Vera.ttf",
          atlasSize = Vec2(256,256)
        },
        fontSize = 24,
        textColor = Color(1,1,1,1),
        showShadow = true,
        shadowOffset = Vec2(1,-1),
        shadowColor = Color(1,1,1,.3),
        halign = "center",
        valign = "center",
        listeners = 
        {
          mouseEnter = function(event)
                         event.target:showFrame(true)
                         log.debug(event.target:className())
                       end,
          mouseLeave = function(event)
                         event.target:showFrame(false)
                         log.debug(event.target:className())
                       end
        }          
      }
    }
  }
}


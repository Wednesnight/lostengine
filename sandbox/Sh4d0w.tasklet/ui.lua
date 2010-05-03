using "lost.guiro.xleft"
using "lost.guiro.xright"
using "lost.guiro.ybottom"
using "lost.guiro.ytop"
using "lost.guiro.wrel"
using "lost.guiro.hrel"
using "lost.guiro.xabs"
using "lost.guiro.yabs"
using "lost.guiro.habs"
using "lost.guiro.wabs"
using "lost.guiro.Bounds"
using "lost.common.Color"

local radius = 20


return dcl.guiro:Screen
{
  listeners = 
  {
    buttonClick = function(event)
      log.debug(event.target.id)
      if event.target.id == "ssao" then
        scene.ssaoEnabled = event.target:pushed()
      elseif event.target.id == "light" then
        scene.lightingEnabled = event.target:pushed()
      elseif event.target.id == "shadow" then
        scene.shadowmapEnabled = event.target:pushed()
      elseif event.target.id == "matcap" then
        scene.matcapEnabled = event.target:pushed()
      end
    end
  },
  dcl.guiro:UserInterface
  {
    id = "ui",
    bounds = Bounds(xleft(), ybottom(), wrel(1), hrel(1)),
    dcl.guiro:Window
    {
      hidden = true,
      id = "hintWindow",
      bounds = Bounds(xleft(), ytop(), wrel(1, -100), habs(40)),
      showBackground = true,
      backgroundColor = Color(1,1,1,.5),
      showFrame = true,
      frameWidth = 4,
      backgroundCornerRadius = radius,
      frameCornerRadius = radius,
      backgroundRoundCorners = {false, false, false, false},
      frameRoundCorners = {true, true, true, true},
      frameShowSides = {false, true, false, false},
      dcl.guiro:Label
      {
        bounds = Bounds(xleft(), ybottom(), wrel(1), hrel(1)),
        text = "hello there",
        fontSize = 16,
        showShadow = false,
        textColor = Color(0,0,0)
      }
    },
    dcl.guiro:Window
    {
      id = "buttons",
      bounds = Bounds(xright(), ytop(), wabs(100), habs(100)),
      showBackground = true,
      backgroundColor = Color(1,1,1,.5),
      showFrame = true,
      frameWidth = 4,
      backgroundCornerRadius = radius,
      frameCornerRadius = radius,
      backgroundRoundCorners = {true, false, false, false},
      frameRoundCorners = {true, true, true, true},
      frameShowSides = {false, true, true, false},
      dcl.guiro:VBox
      {
        id = "stack",
        bounds = Bounds(xright(), ybottom(), wrel(1,-6), hrel(1)),
        mode = "stack",
        spacing = 4,
        halign = "center",
        valign = "center",
        dcl.guiro:Button
        {
          id = "ssao",
          theme = "pebble",
          style = "RoundedRectSmall",
          bounds = Bounds(xabs(50), yabs(50), wabs(80), nil), 
          title = "SSAO (1)",
          mode = "toggle",
        },
        dcl.guiro:Button
        {
          id = "light",
          theme = "pebble",
          style = "RoundedRectSmall",
          bounds = Bounds(xabs(50), yabs(50), wabs(80), nil), 
          title = "Lighting (2)",
          mode = "toggle",
        },
        dcl.guiro:Button
        {
          id = "shadow",
          theme = "pebble",
          style = "RoundedRectSmall",
          bounds = Bounds(xabs(50), yabs(50), wabs(80), nil), 
          title = "Shadows (3)",
          mode = "toggle",
        },
        dcl.guiro:Button
        {
          id = "matcap",
          theme = "pebble",
          style = "RoundedRectSmall",
          bounds = Bounds(xabs(50), yabs(50), wabs(80), nil), 
          title = "MatCap (4)",
          mode = "toggle",
        }, 
      },
      
    }
  }
}
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
    bounds = {"left", "bottom", "1", "1"},
    dcl.guiro:Window
    {
      hidden = true,
      id = "hintWindow",
      bounds = {"left", "top", {"1", -100}, 40}, 
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
        bounds = {"left", "bottom", "1", "1"},
        text = "hello there",
        fontSize = 16,
        showShadow = false,
        textColor = Color(0,0,0)
      }
    },
    dcl.guiro:Window
    {
      id = "buttons",
      bounds = {"right", "top", 100, 100},
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
        bounds = {"right", "bottom", {"1", -6}, "1"}, 
        mode = "stack",
        spacing = 4,
        halign = "center",
        valign = "center",
        dcl.guiro:Button
        {
          id = "ssao",
          theme = "pebble",
          style = "RoundedRectSmall",
          bounds = {50, 50, 80, nil},
          title = "SSAO (1)",
          mode = "toggle",
        },
        dcl.guiro:Button
        {
          id = "light",
          theme = "pebble",
          style = "RoundedRectSmall",
          bounds = {50, 50, 80, nil},
          title = "Lighting (2)",
          mode = "toggle",
        },
        dcl.guiro:Button
        {
          id = "shadow",
          theme = "pebble",
          style = "RoundedRectSmall",
          bounds = {50, 50, 80, nil},
          title = "Shadows (3)",
          mode = "toggle",
        },
        dcl.guiro:Button
        {
          id = "matcap",
          theme = "pebble",
          style = "RoundedRectSmall",
          bounds = {50, 50, 80, nil},
          title = "MatCap (4)",
          mode = "toggle",
        }, 
      },
      
    }
  }
}
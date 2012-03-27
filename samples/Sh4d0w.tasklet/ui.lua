local Color = lost.common.Color

local radius = 20

return lost.guiro.view.View {
  id = "mainView",
  showBackground = true,
  backgroundColor = Color(1,1,1,.5),
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
  subviews = {
    lost.guiro.view.View {
      id = "hints",
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
      subviews = {
        lost.guiro.view.Label {
          bounds = {"left", "bottom", "1", "1"},
          text = "hello there",
          fontSize = 16,
          showShadow = false,
          textColor = Color(0,0,0)
        }
      }
    },
    lost.guiro.view.View {
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
      subviews = {
        lost.guiro.view.View {
          id = "stack",
          bounds = {"right", "bottom", {"1", -6}, "1"}, 
          subviews = {
            lost.guiro.view.Button {
              id = "ssao",
              style="checkboxCandy",
              bounds = {50, 50, 80, 25},
              title = "SSAO (1)",
              mode = "toggle",
            },
            lost.guiro.view.Button {
              id = "light",
              style="checkboxCandy",
              bounds = {50, 50, 80, 25},
              title = "Lighting (2)",
              mode = "toggle",
            },
            lost.guiro.view.Button {
              id = "shadow",
              style="checkboxCandy",
              bounds = {50, 50, 80, 25},
              title = "Shadows (3)",
              mode = "toggle",
            },
            lost.guiro.view.Button {
              id = "matcap",
              style="checkboxCandy",
              bounds = {50, 50, 80, 25},
              title = "MatCap (4)",
              mode = "toggle",
            }, 
          }
        },
      }
    }
  }
}

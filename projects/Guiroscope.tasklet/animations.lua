require("lost.animation")

local Tween = lost.animation.Tween

local reverse = {}
local running = {}
function animate(button, method, bounds1, bounds2)
  if not running[button] then
    running[button] = true

    if reverse[button] then
      reverse[button] = false
      Tween(bounds2, bounds1, {method = method,
                               afterProcess = function(t) button:bounds(lost.guiro.Bounds(bounds2.x, bounds2.y, bounds2.width, bounds2.height)) end,
                               onComplete = function(t) running[button] = false end})
    else
      reverse[button] = true
      Tween(bounds1, bounds2, {method = method,
                               afterProcess = function(t) button:bounds(lost.guiro.Bounds(bounds1.x, bounds1.y, bounds1.width, bounds1.height)) end,
                               onComplete = function(t) running[button] = false end})
    end
  end
end

return lost.guiro.view.View
{
  subviews=
  {
    lost.guiro.view.View
    {    
      bounds = {0,0,"1",{"1",-8}},
      subviews=
      {
        lost.guiro.view.Button
        {
          title = "Linear",
          bounds = {10,650,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.linear, {x = 10, y = 650, width = 100, height = 30}, {x = 110, y = 650, width = 100, height = 30})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "SineIn",
          bounds = {10,600,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.sineIn, {x = 10, y = 600, width = 100, height = 30}, {x = 110, y = 600, width = 100, height = 30})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "SineOut",
          bounds = {10,550,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.sineOut, {x = 10, y = 550, width = 100, height = 30}, {x = 110, y = 550, width = 100, height = 30})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "SineInOut",
          bounds = {10,500,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.sineInOut, {x = 10, y = 500, width = 100, height = 30}, {x = 110, y = 500, width = 100, height = 30})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "QuadIn",
          bounds = {10,450,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.quadIn, {x = 10, y = 450, width = 100, height = 30}, {x = 110, y = 450, width = 100, height = 30})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "QuadOut",
          bounds = {10,400,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.quadOut, {x = 10, y = 400, width = 100, height = 30}, {x = 110, y = 400, width = 100, height = 30})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "QuadInOut",
          bounds = {10,350,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.quadInOut, {x = 10, y = 350, width = 100, height = 30}, {x = 110, y = 350, width = 100, height = 30})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "CubicIn",
          bounds = {10,300,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.cubicIn, {x = 10, y = 300, width = 100, height = 30}, {x = 110, y = 300, width = 100, height = 30})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "CubicOut",
          bounds = {10,250,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.cubicOut, {x = 10, y = 250, width = 100, height = 30}, {x = 110, y = 250, width = 100, height = 30})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "CubicInOut",
          bounds = {10,200,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.cubicInOut, {x = 10, y = 200, width = 100, height = 30}, {x = 110, y = 200, width = 100, height = 30})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "QuartIn",
          bounds = {10,150,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.quartIn, {x = 10, y = 150, width = 100, height = 30}, {x = 110, y = 150, width = 100, height = 30})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "QuartOut",
          bounds = {10,100,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.quartOut, {x = 10, y = 100, width = 100, height = 30}, {x = 110, y = 100, width = 100, height = 30})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "QuartInOut",
          bounds = {10,50,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.quartInOut, {x = 10, y = 50, width = 100, height = 30}, {x = 110, y = 50, width = 100, height = 30})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "QuintIn",
          bounds = {220,650,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.quintIn, {x = 220, y = 650, width = 100, height = 30}, {x = 320, y = 650, width = 100, height = 30})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "QuintOut",
          bounds = {220,600,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.quintOut, {x = 220, y = 600, width = 100, height = 30}, {x = 320, y = 600, width = 100, height = 30})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "QuintInOut",
          bounds = {220,550,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.quintInOut, {x = 220, y = 550, width = 100, height = 30}, {x = 320, y = 550, width = 100, height = 30})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "ExpoIn",
          bounds = {220,500,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.expoIn, {x = 220, y = 500, width = 100, height = 30}, {x = 320, y = 500, width = 100, height = 30})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "ExpoOut",
          bounds = {220,450,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.expoOut, {x = 220, y = 450, width = 100, height = 30}, {x = 320, y = 450, width = 100, height = 30})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "ExpoInOut",
          bounds = {220,400,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.expoInOut, {x = 220, y = 400, width = 100, height = 30}, {x = 320, y = 400, width = 100, height = 30})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "BackIn",
          bounds = {220,350,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.backIn, {x = 220, y = 350, width = 100, height = 30}, {x = 320, y = 350, width = 100, height = 30})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "BackOut",
          bounds = {220,300,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.backOut, {x = 220, y = 300, width = 100, height = 30}, {x = 320, y = 300, width = 100, height = 30})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "BackInOut",
          bounds = {220,250,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.backInOut, {x = 220, y = 250, width = 100, height = 30}, {x = 320, y = 250, width = 100, height = 30})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "ElasticIn",
          bounds = {220,200,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.elasticIn, {x = 220, y = 200, width = 100, height = 30}, {x = 320, y = 200, width = 100, height = 30})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "ElasticOut",
          bounds = {220,150,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.elasticOut, {x = 220, y = 150, width = 100, height = 30}, {x = 320, y = 150, width = 100, height = 30})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "ElasticInOut",
          bounds = {220,100,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.elasticInOut, {x = 220, y = 100, width = 100, height = 30}, {x = 320, y = 100, width = 100, height = 30})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "CircIn",
          bounds = {220,50,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.circIn, {x = 220, y = 50, width = 100, height = 30}, {x = 320, y = 50, width = 100, height = 30})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "CircOut",
          bounds = {430,650,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.circOut, {x = 430, y = 650, width = 100, height = 30}, {x = 530, y = 650, width = 100, height = 30})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "CircInOut",
          bounds = {430,600,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.circInOut, {x = 430, y = 600, width = 100, height = 30}, {x = 530, y = 600, width = 100, height = 30})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "BounceIn",
          bounds = {430,550,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.bounceIn, {x = 430, y = 550, width = 100, height = 30}, {x = 530, y = 550, width = 100, height = 30})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "BounceOut",
          bounds = {430,500,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.bounceOut, {x = 430, y = 500, width = 100, height = 30}, {x = 530, y = 500, width = 100, height = 30})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "BounceInOut",
          bounds = {430,450,100,30},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.bounceInOut, {x = 430, y = 450, width = 100, height = 30}, {x = 530, y = 450, width = 100, height = 30})
            end
          }
        }
      }
    }
  }
}

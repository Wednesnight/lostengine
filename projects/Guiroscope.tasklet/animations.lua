require("lost.animation")

local Tween = lost.animation.Tween

local reverse = {}
local running = {}
local current = {}
function animate(button, method, bounds1, bounds2)
  if not running[button] then
    running[button] = true

    if reverse[button] then
      reverse[button] = false
      current[button] = current[button] or bounds2
      Tween(current[button], bounds1, {method = method,
                               afterProcess = function(t) button:bounds(lost.guiro.Bounds(current[button].x, current[button].y, current[button].width, current[button].height)) end,
                               onComplete = function(t) running[button] = false end})
    else
      reverse[button] = true
      current[button] = current[button] or bounds1
      Tween(current[button], bounds2, {method = method,
                               afterProcess = function(t) button:bounds(lost.guiro.Bounds(current[button].x, current[button].y, current[button].width, current[button].height)) end,
                               onComplete = function(t) running[button] = false end})
    end
  end
end

local mode = {x = 100, y = 16, width = 0, height = 0}

return lost.guiro.view.View
{
  subviews=
  {
    lost.guiro.view.View
    {    
      bounds = {0,0,"1",{"1",-8}},
      subviews=
      {
        lost.guiro.view.RadioGroup
        {
          bounds = {10,700,100,50},
          layout = lost.guiro.layout.Vertical{halign="center",valign="center",spacing = 4},
          subviews = 
          {
            lost.guiro.view.Button
            {
              pushed=true,
              style="radioCandy",
              size="mini",
              title = "Position",
              id="rg1",
              bounds = {50,"top",100,16},
              listeners = {
                buttonClick = function(event) mode = {x = 100, y = 16, width = 0, height = 0} end
              }
            },
            lost.guiro.view.Button
            {
              style="radioCandy",
              size="mini",
              title = "Size",
              id="rg2",
              bounds = {50,{"top",-25},100,16},
              listeners = {
                buttonClick = function(event) mode = {x = 0, y = 0, width = 100, height = 15} end
              }
            }
          }
        },
        lost.guiro.view.Button
        {
          title = "Linear",
          bounds = {10,650,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.linear, {x = 10, y = 650, width = 100, height = 16}, {x = 10 + mode.x, y = 650 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "SineIn",
          bounds = {10,600,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.sineIn, {x = 10, y = 600, width = 100, height = 16}, {x = 10 + mode.x, y = 600 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "SineOut",
          bounds = {10,550,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.sineOut, {x = 10, y = 550, width = 100, height = 16}, {x = 10 + mode.x, y = 550 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "SineInOut",
          bounds = {10,500,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.sineInOut, {x = 10, y = 500, width = 100, height = 16}, {x = 10 + mode.x, y = 500 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "QuadIn",
          bounds = {10,450,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.quadIn, {x = 10, y = 450, width = 100, height = 16}, {x = 10 + mode.x, y = 450 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "QuadOut",
          bounds = {10,400,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.quadOut, {x = 10, y = 400, width = 100, height = 16}, {x = 10 + mode.x, y = 400 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "QuadInOut",
          bounds = {10,350,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.quadInOut, {x = 10, y = 350, width = 100, height = 16}, {x = 10 + mode.x, y = 350 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "CubicIn",
          bounds = {10,300,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.cubicIn, {x = 10, y = 300, width = 100, height = 16}, {x = 10 + mode.x, y = 300 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "CubicOut",
          bounds = {10,250,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.cubicOut, {x = 10, y = 250, width = 100, height = 16}, {x = 10 + mode.x, y = 250 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "CubicInOut",
          bounds = {10,200,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.cubicInOut, {x = 10, y = 200, width = 100, height = 16}, {x = 10 + mode.x, y = 200 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "QuartIn",
          bounds = {10,150,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.quartIn, {x = 10, y = 150, width = 100, height = 16}, {x = 10 + mode.x, y = 150 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "QuartOut",
          bounds = {10,100,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.quartOut, {x = 10, y = 100, width = 100, height = 16}, {x = 10 + mode.x, y = 100 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "QuartInOut",
          bounds = {10,50,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.quartInOut, {x = 10, y = 50, width = 100, height = 16}, {x = 10 + mode.x, y = 50 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "QuintIn",
          bounds = {220,650,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.quintIn, {x = 220, y = 650, width = 100, height = 16}, {x = 220 + mode.x, y = 650 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "QuintOut",
          bounds = {220,600,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.quintOut, {x = 220, y = 600, width = 100, height = 16}, {x = 220 + mode.x, y = 600 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "QuintInOut",
          bounds = {220,550,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.quintInOut, {x = 220, y = 550, width = 100, height = 16}, {x = 220 + mode.x, y = 550 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "ExpoIn",
          bounds = {220,500,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.expoIn, {x = 220, y = 500, width = 100, height = 16}, {x = 220 + mode.x, y = 500 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "ExpoOut",
          bounds = {220,450,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.expoOut, {x = 220, y = 450, width = 100, height = 16}, {x = 220 + mode.x, y = 450 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "ExpoInOut",
          bounds = {220,400,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.expoInOut, {x = 220, y = 400, width = 100, height = 16}, {x = 220 + mode.x, y = 400 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "BackIn",
          bounds = {220,350,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.backIn, {x = 220, y = 350, width = 100, height = 16}, {x = 220 + mode.x, y = 350 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "BackOut",
          bounds = {220,300,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.backOut, {x = 220, y = 300, width = 100, height = 16}, {x = 220 + mode.x, y = 300 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "BackInOut",
          bounds = {220,250,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.backInOut, {x = 220, y = 250, width = 100, height = 16}, {x = 220 + mode.x, y = 250 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "ElasticIn",
          bounds = {220,200,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.elasticIn, {x = 220, y = 200, width = 100, height = 16}, {x = 220 + mode.x, y = 200 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "ElasticOut",
          bounds = {220,150,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.elasticOut, {x = 220, y = 150, width = 100, height = 16}, {x = 220 + mode.x, y = 150 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "ElasticInOut",
          bounds = {220,100,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.elasticInOut, {x = 220, y = 100, width = 100, height = 16}, {x = 220 + mode.x, y = 100 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "CircIn",
          bounds = {220,50,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.circIn, {x = 220, y = 50, width = 100, height = 16}, {x = 220 + mode.x, y = 50 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "CircOut",
          bounds = {430,650,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.circOut, {x = 430, y = 650, width = 100, height = 16}, {x = 430 + mode.x, y = 650 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "CircInOut",
          bounds = {430,600,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.circInOut, {x = 430, y = 600, width = 100, height = 16}, {x = 430 + mode.x, y = 600 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "BounceIn",
          bounds = {430,550,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.bounceIn, {x = 430, y = 550, width = 100, height = 16}, {x = 430 + mode.x, y = 550 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "BounceOut",
          bounds = {430,500,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.bounceOut, {x = 430, y = 500, width = 100, height = 16}, {x = 430 + mode.x, y = 500 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        },
        lost.guiro.view.Button
        {
          title = "BounceInOut",
          bounds = {430,450,100,16},
          listeners = {
            buttonClick = function(event)
              animate(event.target, Tween.bounceInOut, {x = 430, y = 450, width = 100, height = 16}, {x = 430 + mode.x, y = 450 + mode.y, width = 100 + mode.width, height = 16 + mode.height})
            end
          }
        }
      }
    }
  }
}

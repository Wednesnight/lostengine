local Color = lost.common.Color
local Vec2 = lost.math.Vec2

function startup()  
  require("lost.guiro")
  local r = lost.guiro.layer.Rect
  local rr = lost.guiro.layer.dfrr
  local t = lost.guiro.layer.Text
  
  lost.guiro.ui():add
  {
    lost.guiro.view.View
    {
      id = "mainView",
      listeners =
      {
        drop = function (event)
          tasklet:dispatchApplicationEvent(lost.application.SpawnTaskletEvent.create(event.filename))
        end
      },
      sublayers = 
      {
        r{bounds={0,0,"1","1"},id="whiteBackground",filled=true,color=Color(1,1,1)},
        rr{id="leftBox",bounds={{"left",10}, {"bottom",10}, {"0",-15},{"0",-20}},color=Color(1,.5,.2),radius=8,filled=true},
        rr{id="rightBox",bounds={{"right",-10}, {"bottom",10}, {"0",-15},{"0",-20}},color=Color(1,.2,.2),radius=8,filled=true},
        rr{id="centerTextBg",bounds={"center", "-.4", ".7", ".4"},color=Color(1, 1, 1,1),radius=8,filled=true},
        t{id="dropMessage",
          bounds={"center", "-.4", ".6", ".4"},
          font={"Grinched", 28},
          text="Drop tasklet directory here!",
          color=Color(0,0,0),
          shadow=true,
          shadowOffset=Vec2(2,-2),
          shadowColor=Color(0,0,0,.3),
          breakMode = "none",
          },
        rr{id="leftCutout",bounds={"-.10", "center", ".10", ".4"},color=Color(1, 1, 1),radius=8,filled=true},
        rr{id="rightCutout",bounds={"1.1", "center", ".10", ".4"},color=Color(1, 1, 1),radius=8,filled=true},
      }
    }
  }
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)

  require("lost.animation")
  local Tween = lost.animation.Tween
  local Bounds = lost.guiro.Bounds
  local Rect = lost.math.Rect

  local anim = function(target, targetBoundsDef, method, duration, callback)
    local targetBounds = Bounds(unpack(targetBoundsDef))
    local targetRect = targetBounds:rect(target:superRect())
    targetRect = {x = targetRect.x, y = targetRect.y, w = targetRect.width, h = targetRect.height}
    local sourceRect = {x = target.rect.x, y = target.rect.y, w = target.rect.width, h = target.rect.height}

    Tween(sourceRect, targetRect, {method = method or Tween.backOut, duration = duration or 1,
                                   afterProcess = function(t) target:bounds(Bounds(sourceRect.x, sourceRect.y, sourceRect.w, sourceRect.h)) end,
                                   onComplete = function(t) target:bounds(targetBounds) if type(callback) == "function" then callback() end end})
  end

  require("lost.common.CallLater")
  lost.common.callLater(function()
    anim(lost.guiro.ui()("mainView").layer("leftBox"), {{"left",10}, {"bottom",10}, {".5",-15},{"1",-20}}, Tween.elasticOut)
    anim(lost.guiro.ui()("mainView").layer("rightBox"), {{"right",-10}, {"bottom",10}, {".5",-15},{"1",-20}}, Tween.elasticOut, nil, function()
      anim(lost.guiro.ui()("mainView").layer("leftCutout"), {"left", "center", ".10", ".4"})
      anim(lost.guiro.ui()("mainView").layer("rightCutout"), {"right", "center", ".10", ".4"})
      anim(lost.guiro.ui()("mainView").layer("centerTextBg"), {"center", "center", ".7", ".4"}, nil, 0.5)
      anim(lost.guiro.ui()("mainView").layer("dropMessage"), {"center", "center", ".6", ".4"}, nil, 0.5)
    end)
  end)
end

function keyHandler(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end

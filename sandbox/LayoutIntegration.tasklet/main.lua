require("lost.guiro.Bounds")
require "lost.declarative.Context"

using "lost.math.Vec2"
using "lost.math.Rect"
using "lost.application.WindowParams"
using "lost.guiro.Bounds"

using "lost.guiro.xleft"
using "lost.guiro.yabs"
using "lost.guiro.wabs"
using "lost.guiro.habs"

screensize = Vec2(320,480)
windowParams = WindowParams("LayoutIntegration", Rect(0,0,screensize.x, screensize.y))
running = true

function sameRect(a,b)
  local result = true
  if a.x ~= b.x then result = false end
  if a.y ~= b.y then result = false end
  if a.w ~= b.w then result = false end
  if a.h ~= b.h then result = false end
  return result
end

function test(b, parent, resultRect)
  log.debug("testing: "..tostring(b))
  local result = true
  result = b:complete()
  print("parent: "..tostring(parent).." target:"..tostring(resultRect).." actual:"..tostring(b:rect(parent)))
  if parent and result then
    assert(sameRect(b:rect(parent), resultRect))
  end
  
  return result
end

function startup(tasklet)
  tasklet.name = "LayoutIntegration"
  tasklet.waitForEvents = true
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)  
  log.debug("----------------------------------------------")
  log.debug("----------------------------------------------")
  local b1 = Bounds(xleft(),yabs(59), wabs(20), habs(20))
  log.debug("b1 complete: "..tostring(b1:complete()))
  local b2 = Bounds(13,13, 13, 13)
  log.debug("b2 complete: "..tostring(b2:complete()))

  local prect = Rect(12,24,100,100)


  assert(test(Bounds(0,0,50,50), prect, Rect(12,24,50,50)))
  assert(test(Bounds(".5",0,50,50), prect, Rect(62, 24, 50,50)))
  assert(test(Bounds({".5", 10},0,50,50), prect, Rect(72, 24, 50,50)))
  assert(test(Bounds("left",0,50,50), prect, Rect(12,24,50,50)))
  assert(test(Bounds({"left",  10},0,50,50), prect, Rect(22, 24, 50,50)))
  assert(test(Bounds("center",0,50,50), prect, Rect(37, 24, 50,50)))
  assert(test(Bounds({"center", ".75"},0,50,50), prect, Rect(49, 24, 50,50)))
  assert(test(Bounds({"right", 10},0,50,50), prect, Rect(72,24, 50,50)))
  assert(test(Bounds("left","bottom",50,50), prect, Rect(12,24, 50,50)))
  assert(test(Bounds("left","bottom",".5",".25"), prect, Rect(12,24, 50,25)))

  dcl = lost.declarative.Context(tasklet.loader)
  
  v = dcl.guiro:View
  {
    bounds = {"left","top",13,".5"}
  }
  
  return running
end

function update(tasklet)
  return running
end

function shutdown(tasklet)
  return true
end

function keyHandler(event)
  if (event.key == lost.application.K_ESCAPE) then
    running = false
  end
end

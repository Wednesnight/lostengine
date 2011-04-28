local Vec2 = lost.math.Vec2
local Color = lost.common.Color

-- see: http://de.wikipedia.org/wiki/Binomialkoeffizient#Algorithmus_zur_effizienten_Berechnung
function binomial_coefficient(n, k)
  if (k == 0) then
    return 1
  end
  if (2*k > n) then
    return binomial_coefficient(n, n-k)
  else
    local result = n-k+1
    for i = 2,k do
      result = (result * (n-k+i)) / i
    end
    return result
  end
end

function bezier_curve(t, p0, p1, ...)
  local n = 1+arg.n
  local result = (math.pow((1-t), n) * p0) + (binomial_coefficient(n, 1) * math.pow((1-t), (n-1)) * t * p1)
  if n > 1 then
    for nn = 1,arg.n-1 do
      result = result + (binomial_coefficient(n, n-nn) * (1-t) * math.pow(t, (n-nn)) * arg[nn])
    end
    result = result + (math.pow(t, n) * arg[arg.n])
  end
  return result
end

function startup()
  local bmp = lost.bitmap.Bitmap.create(640, 480, lost.bitmap.COMPONENTS_RGBA)
  bmp:clear(Color(0,0,0))
  local step = 0.001
  for t = 0, 1+step, step do
    local point = bezier_curve(t, Vec2(0,0), Vec2(320,480))
    bmp:pixel(point.x-1, point.y-1, Color(1,1,1))
  end
  for t = 0, 1+step, step do
    local point = bezier_curve(t, Vec2(100,100), Vec2(50,300), Vec2(500, 300), Vec2(450, 100))
    bmp:pixel(point.x-1, point.y-1, Color(1,0,0))
  end

  require("lost.guiro")
  lost.guiro.ui():add
  {
    lost.guiro.view.Image
    {
      bitmap = bmp
    }
  }
end

function update()
end

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end

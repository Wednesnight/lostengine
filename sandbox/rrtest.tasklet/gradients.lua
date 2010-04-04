local c5lightGradient = ColorGradient.create()

local c = 238/255
c5lightGradient:add(ColorPoint(0, Color(c,c,c)))
c5lightGradient:add(ColorPoint(.5, Color(1,1,1)))
c5lightGradient:add(ColorPoint(1, Color(1,1,1)))

local c5lightCoord = textureManager:addGradient(c5lightGradient)
log.debug("------ c5lightCoord "..c5lightCoord)
return 
{
  c5light = c5lightCoord
}
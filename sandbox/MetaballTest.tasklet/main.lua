require("lost.declarative.Context")
require("lost.common.Shaders")

using "lost.math.Vec2"
using "lost.math.Rect"
using "lost.common.Color"

local taskletName = "MetaballTest"
local config = require("config")
local running = true

function keyHandler(event)
  if (event.key == lost.application.K_ESCAPE) then
    running = false
  end
end

local m1pos = Vec2(105,130)
local m1rad = 75
local m1col = Color(1,0,0)

local m2pos = Vec2(355,355)
local m2rad = 50
local m2col = Color(0,1,0)

local m3pos = Vec2(350,130)
local m3rad = 35
local m3col = Color(0,0,1)

local mpow = math.pow

function mb(mpos, mrad, x,y)
  local d = math.sqrt(mpow(x-mpos.x, 2) + mpow(y-mpos.y, 2))
  local v = mrad/d
  local falloff = 1 --mpow(mrad-mpow(d,2), 2)
  return v*falloff
end

function startup()
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)  
  dcl = lost.declarative.Context(tasklet.loader)

  log.debug("!!! CALCULATING !!! Please wait ...")
  local bmp = lost.bitmap.Bitmap.create(config.window.width,
                                        config.window.height,
                                        lost.bitmap.COMPONENTS_RGBA)
  bmp:clear(Color(1,0,0))
  local max = math.max
  local min = math.min
  for x=0,config.window.width-1,1 do
    for y=0,config.window.height-1,1 do
      local v1 = mb(m1pos, m1rad,x,y)
      local v2= mb(m2pos, m2rad,x,y)
      local v3= mb(m3pos, m3rad,x,y)
      local c1 = m1col * v1
      local c2 = m2col * v2
      local c3 = m3col * v3
      local cs = Color()
      cs.r = min(1,max(0,c1.r + c2.r + c3.r))
      cs.g = min(1,max(0,c1.g + c2.g + c3.g))
      cs.b = min(1,max(0,c1.b + c2.b + c3.b))
      
      local m = 1 --max(max(max(0,cs.r), cs.g), cs.b)
      local s= min(1,max(0,v1+v2+v3)) -- clamp to [0,1]
      if s < .96 then s = 0 end
      cs.r = (cs.r/m)*s
      cs.g = (cs.g/m)*s
      cs.b = (cs.b/m)*s
      cs.a = 1
      bmp:pixel(x,y,cs)
    end
  end
  
  local tex = lost.gl.Texture.create(bmp, lost.gl.Texture.Params())

  tasklet.renderNode:add(
    dcl.rg:Node
    {
      dcl.rg:ClearColor
      {
        color = Color(0,0,0,0)
      },
      dcl.rg:Clear
      {
        mask = gl.GL_COLOR_BUFFER_BIT+gl.GL_DEPTH_BUFFER_BIT
      },
      dcl.rg:DepthTest{false},
      dcl.rg:Camera2D
      {
        viewport = Rect(0,0,config.window.width, config.window.height)
      },    
      dcl.rg:Draw
      {
        mesh = dcl.mesh:Quad
        {
          texture = tex,
          material = 
          {
            blend = true,
            color = Color(1,1,1),
            shader = lost.common.Shaders.textureShader()
          }
        }
      }
    }
  )
  return true
end

function update()
  return running
end



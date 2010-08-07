require "lost.declarative.Context"

local loadShader = lost.gl.loadShader
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local Rect = lost.math.Rect
local Color = lost.common.Color
local Camera2D = lost.camera.Camera2D
local Camera3D = lost.camera.Camera3D
local WindowParams = lost.application.WindowParams
local MatrixTranslation = lost.math.MatrixTranslation

imageSize = Vec2(400, 392)
screenSize = Vec2(imageSize.x*2, imageSize.y)

hasWindow = true
windowParams = WindowParams("Schwurbel", Rect(50, 200, screenSize.width, screenSize.height))

function startup()
  tasklet.name = "Schwurbel"
  log.debug("startup")

  running = true
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)
  tasklet.eventDispatcher:addEventListener(lost.application.MouseEvent.MOUSE_MOVE, updateShader)

  animated = true
  angle = 0
  passedSec = lost.platform.currentTimeSeconds()

  dcl = lost.declarative.Context(tasklet.loader)
  rootNode = dcl.rg:Node
  {
    dcl.rg:ClearColor
    {
      color = Color(1,1,1,1)
    },
    dcl.rg:Clear
    {
      mask = gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT
    },
    dcl.rg:Camera2D
    {
      viewport = Rect(0,0,screenSize.x,screenSize.y)
    },
    dcl.rg:Draw
    {
      name = "original",
      mesh = dcl.mesh:Quad
      {
        filename = "foo.jpg",
        size = imageSize,
        flip = true
      }
    },
    dcl.rg:Draw
    {
      name = "shadered",
      mesh = dcl.mesh:Quad
      {
        texture = dcl.gl:Texture
        {
          filename = "foo.jpg",
          params = dcl.gl:TextureParams
          {
            minFilter = gl.GL_LINEAR,
            magFilter = gl.GL_LINEAR
          }
        },
        size = imageSize,
        flip = true,
        material = 
        {
          shader = dcl.gl:Shader
          {
            filename = "shader"
          },
          blend = true
        },
        transform = MatrixTranslation(Vec3(imageSize.x,0,0))
      }
    }
  }
  
  mesh = rootNode:recursiveFindByName("shadered").mesh
  local tex = lost.gl.Texture.create(tasklet.loader:load("foo.jpg"), lost.gl.Texture.Params())
  tex:filter(gl.GL_LINEAR)
  mesh.material:addTexture(tex)

  shader = mesh.material.shader
  shader:enable()
  shader:setFloat("shader", 1)
  shader:setFloat("factor", 0)
  shader:set("pos", Vec2(0,0))
  shader:disable()

  tasklet.renderNode:add(rootNode)

  return true;
end

function update()
  local currentSec = lost.platform.currentTimeSeconds()
  local delta = currentSec - passedSec
  if animated then
    angle = math.fmod(delta*250+angle, 360)
    shader:enable()
    shader:setFloat("factor", math.cos(math.rad(angle)) + 3.0)
    shader:disable()
  end
  passedSec = currentSec

  return running
end

function setShader(which)
  shader:enable()
  shader:setFloat("shader", which)
  shader:disable()
end

function keyHandler(event)
  if (event.key == lost.application.K_ESCAPE) then
    running = false
  elseif (event.key == lost.application.K_SPACE) then
    animated = not animated
  elseif (event.key == lost.application.K_1) then
    setShader(1)
  elseif (event.key == lost.application.K_2) then
    setShader(2)
  elseif (event.key == lost.application.K_3) then
    setShader(3)
  elseif (event.key == lost.application.K_4) then
    setShader(4)
  elseif (event.key == lost.application.K_5) then
    setShader(5)
  elseif (event.key == lost.application.K_6) then
    setShader(6)
  elseif (event.key == lost.application.K_7) then
    setShader(7)
  elseif (event.key == lost.application.K_8) then
    setShader(8)
  end
end

function updateShader(event)
  if event.pos.x > imageSize.x then
    shader:enable()
    shader:set("pos", Vec2(((event.pos.x-imageSize.x)/imageSize.x)*2, ((imageSize.y-event.pos.y)/imageSize.y)*2))
    shader:disable()
  end
end

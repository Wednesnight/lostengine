using "lost.common.Color"
using "lost.math.Vec2"
using "lost.math.Vec3"
using "lost.math.Rect"

local camPos = Vec3(5,2,2)
local controller = require("controller")
controller.sphereSubdivisions = 16
local sphere = lost.mesh.Sphere.create(1.0, controller.sphereSubdivisions)
sphere.material.shader = dcl.gl:Shader
{
  filename = "light"
}
sphere.material.shader:enable()
sphere.material.shader:set("lightPosition", camPos)
sphere.material.shader:disable()
controller.debugModel = dcl.rg:Draw
{
  name = "model",
  mesh = sphere
}

return
{
  dcl.rg:ClearColor
  {
    color = Color(0,0,0)
  },
  dcl.rg:Clear
  {
    mask = gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT
  },
  dcl.rg:Camera3D
  {
    viewport = Rect(0, 0, 320, 240),
    position = camPos,
    fovY = 45.0,
    depth = Vec2(1.0, 1000.0),
    target = Vec3(0, 0, 0),
    stickToTarget = true
  },
  dcl.rg:DepthTest
  {
    true
  },
  controller.debugModel
}

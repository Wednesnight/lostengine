local Color = lost.common.Color
local Rect = lost.math.Rect
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local MatrixTranslation = lost.math.MatrixTranslation
local MatrixRotY = lost.math.MatrixRotY
local MatrixRotZ = lost.math.MatrixRotZ

require("mesh")

function startup()
--  require("lost/guiro")
--  createMesh()
  tasklet.clearNode.active = false

  rootNode = lost.rg.Node.create()
  rootNode:add(lost.rg.ClearColor.create(Color(0,0,0,1)))
  rootNode:add(lost.rg.Clear.create(gl.GL_COLOR_BUFFER_BIT+gl.GL_DEPTH_BUFFER_BIT))
  cam = lost.camera.Camera3D.create(Rect(0,0,tasklet.window.size.width,tasklet.window.size.height))
  cam:depth(Vec2(0.001,1000))
  cam:position(Vec3(0,0,50))
  rootNode:add(lost.rg.Camera.create(cam))

  s = "Supercalifragilistic"
  font = tasklet.fontManager:getFont("Grinched",20)
  txtmesh = lost.font.render(s,font,false,0)
  shader = lost.gl.loadShader(tasklet.loader, "lost/resources/glsl/basic_texture")
  txtmesh.material.shader = shader
  txtmesh.material:getTexture(0):filter(gl.GL_LINEAR)
  txtmesh.transform = MatrixTranslation(Vec3(-100,-.5,-1))*MatrixRotY(20)*MatrixRotZ(10)
  rootNode:add(lost.rg.DepthTest.create(true))
  rootNode:add(lost.rg.Draw.create(txtmesh))
--  rootNode:add(lost.rg.DepthTest.create(false))
  rootNode:add(lost.rg.Draw.create(createMesh()))

  tasklet.renderNode:add(rootNode)

  tasklet.eventDispatcher:addEventListener(lost.application.ResizeEvent.TASKLET_WINDOW_RESIZE,resize)
end

function update()
end

function resize(event)
  cam:viewport(Rect(0,0,tasklet.window.size.width,tasklet.window.size.height))
end

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end
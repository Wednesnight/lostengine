local Color = lost.common.Color
local Rect = lost.math.Rect
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local MatrixTranslation = lost.math.MatrixTranslation
local MatrixRotY = lost.math.MatrixRotY
local MatrixRotZ = lost.math.MatrixRotZ

function startup()
--  require("lost/guiro")

  tasklet.clearNode.active = false

  rootNode = lost.rg.Node.create()
  rootNode:add(lost.rg.ClearColor.create(Color(1,0,0,1)))
  rootNode:add(lost.rg.Clear.create(gl.GL_COLOR_BUFFER_BIT+gl.GL_DEPTH_BUFFER_BIT))
  cam = lost.camera.Camera3D.create(Rect(0,0,tasklet.window.size.width,tasklet.window.size.height))
  cam:depth(Vec2(0,1000))
  rootNode:add(lost.rg.Camera.create(cam))

  s = "Supercalifragilistic"
  font = tasklet.fontManager:getFont("Grinched",100)
  txtmesh = lost.font.render(s,font,false,0)
  shader = lost.gl.loadShader(tasklet.loader, "lost/resources/glsl/basic_texture")
  txtmesh.material.shader = shader
  txtmesh.material:getTexture(0):filter(gl.GL_LINEAR)
  txtmesh.transform = MatrixTranslation(Vec3(-100,-.5,-700))*MatrixRotY(49)*MatrixRotZ(37)
  rootNode:add(lost.rg.Draw.create(txtmesh))

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
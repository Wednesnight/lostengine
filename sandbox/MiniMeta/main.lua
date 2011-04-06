local Color = lost.common.Color
local Rect = lost.math.Rect
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local MatrixTranslation = lost.math.MatrixTranslation
local MatrixRotY = lost.math.MatrixRotY
local MatrixRotX = lost.math.MatrixRotX
local MatrixRotZ = lost.math.MatrixRotZ

require("Cube")

mbdown = false
roty = 0
rotx = 0

timeaccu = 0
fps = 60
timestep = 1/fps
rad1 = 0

pos1start = Vec3(1.2,1.2,1.2)
pos1end = Vec3(.3,.3,.3)
pos1time = 10
pos1timecur = 0
pos1delta = pos1end-pos1start
pos1current = Vec3(.5,.5,.5)
pos1dir = 1 -- 2


function applyPreset(shader,presetTable)
  tasklet.window.context:shader(shader)
  for k,v in pairs(presetTable) do
    if type(v) == "table" then
      local name = v[1]
      local val = v[2]
      local ptype = v[3]
      if ptype == nil then shader:set(name,val)
      elseif ptype == "float" then shader:setFloat(name,val)
      elseif ptype == "int" then shader:setInt(name,val)
      elseif ptype == "bool" then shader:setBool(name,val)
      end
    end
  end
end

function changePresetHandler(event)
  log.debug("preset value "..tostring(event.indexPath[1]))
  applyPreset(metaShader,presets[event.indexPath[1]])
end

function startup()
  require("lost/guiro")
  presets = require("presets")
  popupitems = {}
  for k,v in pairs(presets) do
    table.insert(popupitems,{title=v.name})
    log.debug("found preset "..v.name)
  end
  lost.guiro.ui():add{require("ui")}
  presetButton = lost.guiro.ui():recursiveFindById("presets")
  presetButton:items(popupitems)
  presetButton:addEventListener("menuItemSelected",changePresetHandler)
  
  controlPanel = lost.guiro.ui():recursiveFindById("controlPanel")
  tasklet.clearNode.active = false

  metaShader = lost.gl.loadShader(tasklet.loader,"meta");
  tasklet.window.context:shader(metaShader)
  metaShader:setFloat("drama",80)
  metaShader:setFloat("ambience",0)

  rootNode = lost.rg.Node.create()
  rootNode:add(lost.rg.ClearColor.create(Color(0,0,0,1)))
  rootNode:add(lost.rg.Clear.create(gl.GL_COLOR_BUFFER_BIT+gl.GL_DEPTH_BUFFER_BIT))
  cam = lost.camera.Camera3D.create(Rect(0,0,tasklet.window.size.width,tasklet.window.size.height))
  cam:depth(Vec2(0.100,1000))
  cam:position(Vec3(100,00,200))
  cam:target(Vec3(0,0,0))
  rootNode:add(lost.rg.Camera.create(cam))

  local cubeSize = 200

  cube = meta.Cube{color=Color(.1,.6,.0),size=200,numPlanes=70}

  rootNode:add(lost.rg.DepthTest.create(true))
  rootNode:add(cube.renderNode)


  tasklet.renderNode:add(rootNode)

  tasklet.eventDispatcher:addEventListener(lost.application.ResizeEvent.TASKLET_WINDOW_RESIZE,resize)
  tasklet.eventDispatcher:addEventListener(lost.application.MouseEvent.MOUSE_MOVE,mouseMove)
  tasklet.eventDispatcher:addEventListener(lost.application.MouseEvent.MOUSE_UP,mouseUp)
  tasklet.eventDispatcher:addEventListener(lost.application.MouseEvent.MOUSE_DOWN,mouseDown)
  tasklet.eventDispatcher:addEventListener(lost.application.MouseEvent.MOUSE_SCROLL,mouseScroll)
end

function updateStuff()
  pos1current = pos1start+pos1delta*(pos1timecur/pos1time)
  tasklet.window.context:shader(metaShader)
  metaShader:set("pos1",pos1current)
  if pos1timecur < 0 then dir = 1 
  elseif pos1timecur > pos1time then dir = 2
  end
  
  if dir == 1 then 
    pos1timecur =  pos1timecur + timestep
  else
    pos1timecur =  pos1timecur - timestep
  end
end

function update(delta)
  timeaccu = timeaccu + delta
  if timeaccu >= timestep then
    while timeaccu >= timestep do
      updateStuff()
      timeaccu = timeaccu - timestep
    end
  end
end

function mouseMove(event)
  if mbdown then
    roty = roty + (event.pos.x-mousex)
    rotx = rotx + (event.pos.y-mousey)
    mousex = event.pos.x
    mousey = event.pos.y
    cube:transform(MatrixRotY(roty)*MatrixRotX(rotx))
  end
end

function mouseScroll(event)
  local dir = cam:direction()
  local pos = cam:position()
  lost.math.normalise(dir)
  pos = pos + (dir * event.scrollDelta.y)
  cam:position(pos)
end

function mouseUp(event)
  mbdown = false
end

function mouseDown(event)
  if not controlPanel:containsCoord(event.pos) then
    mbdown = true
    mousex = event.pos.x
    mousey = event.pos.y
  end
end

function resize(event)
  cam:viewport(Rect(0,0,tasklet.window.size.width,tasklet.window.size.height))
end

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  elseif event.key == lost.application.K_TAB and event.pressed then
    controlPanel:hidden(not controlPanel:hidden())
  end
end
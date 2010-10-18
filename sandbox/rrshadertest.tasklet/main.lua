require("lost.declarative.Context")
require("lost.common.Shaders")

using "lost.math.Vec2"
using "lost.math.Vec3"
using "lost.math.Rect"
using "lost.common.Color"
using "lost.bitmap.Bitmap"
using "lost.math.MatrixTranslation"
using "lost.math.MatrixRotZ"

config = require("config")
running = true
rootNode = nil
dcl = nil

sum = 0
curwidth = 3

shaderCache = {}

function loadShader(name)
  local result = shaderCache[name]
  if not result then
    result = lost.gl.loadShader(tasklet.loader, name)
    shaderCache[name] = result
  end
  return result
end

function buildRRShaderCacheKey(f, rc, s)
  local result = tostring(f).."-("..tostring(rc.tl).."-"..tostring(rc.tr).."-"..tostring(rc.bl).."-"..tostring(rc.br)..")"
  result = result.."-("..tostring(s.top).."-"..tostring(s.bottom).."-"..tostring(s.left).."-"..tostring(s.right)..")"
  return result
end

-- builds a shader that implements a rounded rectangle with the given configuration
-- switching off sides disables adjacent round corners
-- shaders are cached and reuses
--
-- roundCorners is a table with the following keys set to either true or false
-- not all keys need to be present and will default to true if not set
-- roundCorners = {tl=true, tr=true, bl=true, br=true}
--
-- sides is a table with the following keys set to either true or false
-- not all keys need to be present and will default to true if not set
-- sides = {top = true, bottom=true, left=true, right=true}
function buildRRShader(filled, roundCorners, sides)
  local result = nil
  
  -- create local tables for configuration to handle nil params and so we don't modify incoming configuration
  local rc = {}
  local s = {}
  
  if roundCorners.tl ~= nil then rc.tl = roundCorners.tl else rc.tl = true end
  if roundCorners.tr ~= nil then rc.tr = roundCorners.tr else rc.tr = true end
  if roundCorners.bl ~= nil then rc.bl = roundCorners.bl else rc.bl = true end
  if roundCorners.br ~= nil then rc.br = roundCorners.br else rc.br = true end

  if sides.top ~= nil then s.top = sides.top else s.top = true end
  if sides.bottom ~= nil then s.bottom = sides.bottom else s.bottom = true end
  if sides.left ~= nil then s.left = sides.left else s.left = true end
  if sides.right ~= nil then s.right = sides.right else s.right = true end
  
  -- switching off sides affects the corners
  -- all corners adjacent to a disabled side will be set to NOT round
  if not s.top then
    rc.tl = false
    rc.tr = false
  end
  if not s.left then
    rc.tl = false
    rc.bl = false
  end
  if not s.right then
    rc.tr = false
    rc.br = false
  end
  if not s.bottom then
    rc.bl = false
    rc.br = false
  end
  
  local cacheKey = buildRRShaderCacheKey(filled, rc, s)
  log.debug("-- CACHEKEY: '"..cacheKey.."'")
  
  local shader = ""
  
  if filled then
    local imports = "#import \"disc.fsp\"\n#import \"box.fsp\""
    
  else
    local imports = "#import \"ring.fsp\"\n#import \"box.fsp\""
    
  end
  return result
end

function createQuad(size)
  local layout = lost.gl.BufferLayout()
  layout:add(gl.ET_vec2_f32, gl.UT_position, 0)
  layout:add(gl.ET_vec2_f32, gl.UT_texcoord0, 0)
  local result = lost.mesh.Mesh.create(layout, gl.ET_u16)
  result.indexBuffer.drawMode = gl.GL_TRIANGLES
  local numVerts = 4
  local numTris = 2
  local numIndicesPerTri = 3
  local numIndices = numTris * numIndicesPerTri
  result:resetSize(numVerts, numIndices)

  result:set(0,gl.UT_position, Vec2(0,0))
  result:set(1,gl.UT_position, Vec2(size.x,0))
  result:set(2,gl.UT_position, Vec2(size.x,size.y))
  result:set(3,gl.UT_position, Vec2(0,size.y))

  result:set(0,gl.UT_texcoord0, Vec2(0,0))
  result:set(1,gl.UT_texcoord0, Vec2(1,0))
  result:set(2,gl.UT_texcoord0, Vec2(1,1))
  result:set(3,gl.UT_texcoord0, Vec2(0,1))

  result:setU16(0,gl.UT_index, 0)
  result:setU16(1,gl.UT_index, 1)
  result:setU16(2,gl.UT_index, 2)
  result:setU16(3,gl.UT_index, 2)
  result:setU16(4,gl.UT_index, 3)
  result:setU16(5,gl.UT_index, 0)
  
  result.material.uniforms = lost.gl.UniformBlock.create()
  result.material:blendPremultiplied()
  
  return result
end

function createDisc(col, pos, radius)
  local size = Vec2(radius*2, radius*2)
  local result = createQuad(size)
  result.material.color = col
  result.material.shader = loadShader("disc")
  result.material.uniforms:set("size", size)
  result.material.uniforms:set("center", Vec2(radius-1, radius-1))
  result.material.uniforms:setFloat("radius", radius)
  result.transform = MatrixTranslation(Vec3(pos.x,pos.y,0))  
  return result
end

function createRing(col, pos, radius, width)
  local size = Vec2(radius*2, radius*2)
  local result = createQuad(size)
  result.material.color = col
  result.material.shader = loadShader("ring")
  result.material.uniforms:set("size", size)
  result.material.uniforms:set("center", Vec2(radius-1, radius-1))
  result.material.uniforms:setFloat("radius", radius)
  result.material.uniforms:setFloat("width", width)
  result.transform = MatrixTranslation(Vec3(pos.x,pos.y,0))  
  return result
end

function createRoundedRect(col, rect, radius)
  local size = Vec2(rect.width, rect.height)
  local result = createQuad(size)

  result.material.color = col
  result.material.shader = loadShader("roundedRect")
  result.material.uniforms:set("size", size)
  result.material.uniforms:setFloat("radius", radius)
  result.transform = MatrixTranslation(Vec3(rect.x,rect.y,0))
--  result.material:blendOff();
  
  return result
end

function createRoundedRectFrame(col, rect, radius, width)
  local size = Vec2(rect.width, rect.height)
  local result = createQuad(size)

  result.material.color = col
  result.material.shader = loadShader("roundedRectFrame")
  result.material.uniforms:set("size", size)
  result.material.uniforms:setFloat("radius", radius)
  result.material.uniforms:setFloat("width", width)
  result.transform = MatrixTranslation(Vec3(rect.x,rect.y,0))
  
  return result
end

function createBox(col, rect)
  local size = Vec2(rect.width, rect.height)
  local result = createQuad(size)
  result.material.color = col
  result.material.shader = loadShader("box")
  result.material.uniforms:set("size", size)
  result.transform = MatrixTranslation(Vec3(rect.x,rect.y,0))*MatrixRotZ(-87)
  result.material:blendPremultiplied()
  return result
end

function iqcreateRoundedRect(col, rect, radius)
  local result = createRoundedRect(col, rect, radius)
  result.material.shader = loadShader("iqrr")
  return result
end

function iqcreateRoundedRectFrame(col, rect, radius, width)
  local result = createRoundedRectFrame(col, rect, radius, width)
  result.material.shader = loadShader("iqrrf")
  return result
end

function createComboRectNode(col1, col2, rect, radius, width)
  local result = dcl.rg:Node
  {
    dcl.rg:Draw{mesh = createRoundedRect(col1, rect, radius)},
    dcl.rg:Draw{mesh = createRoundedRectFrame(col2, rect, radius, width)},
  }
  return result
end

function iqcreateComboRectNode(col1, col2, rect, radius, width)
  local result = dcl.rg:Node
  {
    dcl.rg:Draw{mesh = iqcreateRoundedRect(col1, rect, radius)},
    dcl.rg:Draw{mesh = iqcreateRoundedRectFrame(col2, rect, radius, width)},
  }
  return result
end

--function roundedRect(col, rect, radius, )

function startup()
  dcl = lost.declarative.Context(tasklet.loader)
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)

  local s = buildRRShader(true, {}, {})

  local white = Color(1,1,1,1)
  local red = Color(1,0,0,1)
  local blue = Color(0,0,1,1)
  local gray1 = Color(.2, .2, .2, 1)
  local gray2 = Color(.8, .8, .8, 1)
  local green = Color(0, 1, 0, 1)

  local r = 4
  local w = 2

  local left = 20
  local right = 320

  rootNode = dcl.rg:Node
  {
    dcl.rg:ClearColor { color = Color(92/255,174/255,210/255,1) },
    dcl.rg:Clear { mask = gl.GL_COLOR_BUFFER_BIT },
    dcl.rg:DepthTest{false},
    dcl.rg:Camera2D { viewport = Rect(0,0,config.window.width, config.window.height) },    

    createComboRectNode(gray2, gray1, Rect(left,390,200,200), 27, 2),
    iqcreateComboRectNode(gray2, gray1, Rect(right,390,200,200), 27, 2),
--    dcl.rg:Draw { mesh = createDisc(white, Vec2(600,300), 47) },
--    dcl.rg:Draw { mesh = createRing(red, Vec2(600,300), 47, 2) },
    
    dcl.rg:Draw { mesh = createRoundedRectFrame(white, Rect(20,180, 10, 10), r,w) },
    dcl.rg:Draw { mesh = createRoundedRectFrame(white, Rect(20,20, 16, 16), r,w) },
    dcl.rg:Draw { mesh = createRoundedRectFrame(white, Rect(100,50, 64, 64), r,w) },
    dcl.rg:Draw { mesh = createRoundedRectFrame(white, Rect(10,100, 13, 13), r,w) },
    dcl.rg:Draw { mesh = createRoundedRectFrame(white, Rect(200,200, 57, 57), r,w) },
    dcl.rg:Draw { mesh = createRoundedRectFrame(white, Rect(420,200, 64, 46), r,w) },

    createComboRectNode(gray2, gray1, Rect(left, 350, 120, 20), r, 1),
    iqcreateComboRectNode(gray2, gray1, Rect(right, 350, 120, 20), r, 1),
    
    dcl.rg:Draw { mesh = createRoundedRectFrame(white, Rect(left,230, 100, 100), r,w) },
    dcl.rg:Draw { mesh = iqcreateRoundedRectFrame(white, Rect(right,230, 100, 100), r,w) },

--    dcl.rg:Draw { mesh = createRoundedRect(white, Rect(left,150, 80, 50), r) },
--    dcl.rg:Draw { mesh = iqcreateRoundedRect(white, Rect(right,150, 80, 50), r) },

    dcl.rg:Draw { mesh = createRoundedRectFrame(white, Rect(left,30, 100, 100), r,1) },
    dcl.rg:Draw { mesh = iqcreateRoundedRectFrame(white, Rect(right, 30, 100, 100), r,1) },

    dcl.rg:Draw { mesh = createDisc(white, Vec2(600,300), 47) },

    dcl.rg:Draw { mesh = iqcreateRoundedRectFrame(white, Rect(right,180, 10, 10), r,w) },

    dcl.rg:Draw { mesh = createBox(white, Rect(right,10, 400, 400)) },
  }    
  tasklet.renderNode:add(rootNode)

  return running
end

function update(dt)
  --log.debug(dt)
  
  return running
end

function keyHandler(event)
  if (event.key == lost.application.K_ESCAPE) then
    running = false
  end
end


module("lost.common", package.seeall)

require("lost.common.Class")

lost.common.Class "lost.common.ShaderFactory" {}

function ShaderFactory:constructor(loader)
  self.loader = loader
  self.cache = {}
  self.roundedRectVsSource = self.loader:load("shaders/roundedRect.vs"):str()
  self.roundedRectFrameVsSource = self.loader:load("shaders/roundedRectFrame.vs"):str()  
end

function ShaderFactory:loadShader(name)
  local shader = self.cache[name]
  if not shader then
    shader = lost.gl.loadShader(self.loader, name)
    self.cache[name] = shader
  end
  return shader
end

function ShaderFactory:ring()
  return self:loadShader("shaders/ring")
end

function ShaderFactory:disc()
  return self:loadShader("shaders/disc")
end

function ShaderFactory:box()
  return self:loadShader("shaders/box")
end

function ShaderFactory:iqrr()
  return self:loadShader("shaders/iqrr")
end

function ShaderFactory:iqrrf()
  return self:loadShader("shaders/iqrrf")
end

-- f = filled
-- rc = roundedCorners
-- s = sides
function ShaderFactory:rrCacheKey(f, rc, s)
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
function ShaderFactory:roundedRect(filled, roundCorners, sides)
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
  
  local cacheKey = self:rrCacheKey(filled, rc, s)
--  log.debug("-- CACHEKEY: '"..cacheKey.."'")

  result = self.cache[cacheKey]
  if result then return result end

  local parts = {} -- receives the vars that need to bee added and clamped for the final result
  
  local shader = ""
  
  if filled then
    shader = shader .. [[

uniform vec4 color;
uniform vec2 size;
uniform float radius;
varying vec2 tc0;

#import "shaders/disc.fsp"
#import "shaders/box.fsp"

float roundedRect(vec2 lpc, vec2 size, float r)
{
  float mr = min(min(size.x/2.0, size.y/2.0), r);

]]
  else
    shader = shader .. [[
uniform vec4 color;
uniform vec2 size;
uniform float radius;
uniform float width;
varying vec2 tc0;

#import "shaders/ring.fsp"
#import "shaders/box.fsp"

float roundedRectFrame(vec2 lpc, vec2 size, float r, float width)
{
  float mr = min(min(size.x/2.0, size.y/2.0), r);

]]
  end
  
  if filled then
    if rc.tl then shader = shader.."  float tl = disc(lpc, vec2(mr-1.0, size.y-mr-1.0), mr);\n";table.insert(parts, "tl") end
    if rc.tr then shader = shader.."  float tr = disc(lpc, vec2(size.x-mr-1.0, size.y-mr-1.0), mr);\n";table.insert(parts, "tr") end
    if rc.bl then shader = shader.."  float bl = disc(lpc, vec2(mr-1.0, mr-1.0), mr);\n";table.insert(parts, "bl") end
    if rc.br then shader = shader.."  float br = disc(lpc, vec2(size.x-mr-1.0, mr-1.0), mr);\n";table.insert(parts, "br") end

    table.insert(parts, "top")
    table.insert(parts, "mid")
    table.insert(parts, "bot")

--    float bot = box(lpc, vec2(mr-1.0, 0), vec2(size.x-mr-1.0, mr-1.0));

    -- top section
    if rc.tl and rc.tr then
      shader = shader .. "  float top = box(lpc, vec2(mr-1.0, size.y-1.0-mr), vec2(size.x-mr-1.0, size.y));\n"
    elseif rc.tl and not rc.tr then
      shader = shader .. "  float top = box(lpc, vec2(mr-1.0, size.y-1.0-mr), vec2(size.x, size.y));\n"
    elseif not rc.tl and rc.tr then
      shader = shader .. "  float top = box(lpc, vec2(0, size.y-1.0-mr), vec2(size.x-mr-1.0, size.y));\n"
    elseif not rc.tl and not rc.tr then
      shader = shader .. "  float top = box(lpc, vec2(0, size.y-1.0-mr), vec2(size.x, size.y));\n"
    end
    
    -- mid section
    -- always the same
    shader = shader .. "  float mid = box(lpc, vec2(0, mr-1.0), vec2(size.x, size.y-mr-1.0));\n"
    
    -- bottom section
    if rc.bl and rc.br then
      shader = shader .. "  float bot = box(lpc, vec2(mr-1.0, 0), vec2(size.x-mr-1.0, mr-1.0));\n"
    elseif rc.bl and not rc.br then
      shader = shader .. "  float bot = box(lpc, vec2(mr-1.0, 0), vec2(size.x, mr-1.0));\n"
    elseif not rc.bl and rc.br then
      shader = shader .. "  float bot = box(lpc, vec2(0, 0), vec2(size.x-mr-1.0, mr-1.0));\n"
    elseif not rc.bl and not rc.br then
        shader = shader .. "  float bot = box(lpc, vec2(0, 0), vec2(size.x, mr-1.0));\n"
    end
    
    -- assemble parts
    shader = shader .. "  float f = 0.0"
    for k,v in pairs(parts) do
      shader = shader .. "+" .. v
    end
    shader = shader..";\n"
    shader = shader .. "  return clamp(f, 0.0, 1.0);\n}"
    
    shader = shader .. [[

    
vec2 localPixelCoord() 
{
  return tc0*(size-vec2(1,1));
}

void main(void)
{ 
  float f = roundedRect(localPixelCoord(), size, radius);
  gl_FragColor = color*f;
}
      
]]
  else    
    -- frame rr

    -- corners
    if rc.tl then shader=shader.."  float tl = quadring(lpc, vec2(mr-1.0, size.y-mr-1.0), mr, width, bvec2(false, true));\n"; table.insert(parts, "tl") end
    if rc.tr then shader=shader.."  float tr = quadring(lpc, vec2(size.x-mr-1.0, size.y-mr-1.0), mr, width, bvec2(true, true));\n";table.insert(parts, "tr") end 
    if rc.bl then shader=shader.."  float bl = quadring(lpc, vec2(mr-1.0, mr-1.0), mr, width, bvec2(false, false));\n";table.insert(parts, "bl") end 
    if rc.br then shader=shader.."  float br = quadring(lpc, vec2(size.x-mr-1.0, mr-1.0), mr, width, bvec2(true, false));\n";table.insert(parts, "br") end 

    -- sides
    if s.top then
      if rc.tl and rc.tr then shader=shader.."  float topl = box(lpc, vec2(mr-1.0, size.y-1.0-width), vec2(size.x-mr-1.0, size.y));\n"
      elseif rc.tl and not rc.tr then shader=shader.."  float topl = box(lpc, vec2(mr-1.0, size.y-1.0-width), vec2(size.x, size.y));\n"
      elseif not rc.tl and rc.tr then shader=shader.."  float topl = box(lpc, vec2(0.0, size.y-1.0-width), vec2(size.x-mr-1.0, size.y));\n"
      elseif not rc.tl and not rc.tr then shader=shader.."float topl = box(lpc, vec2(0.0, size.y-1.0-width), vec2(size.x, size.y));\n" 
      end
      table.insert(parts, "topl")
    end

    if s.bottom then
      if rc.bl and rc.br then shader=shader.."  float botl = box(lpc, vec2(mr-1.0, 0), vec2(size.x-mr-1.0, width));\n"
      elseif rc.bl and not rc.br then shader=shader.."  float botl = box(lpc, vec2(mr-1.0, 0), vec2(size.x.0, width));\n"
      elseif not rc.bl and rc.br then shader=shader.."  float botl = box(lpc, vec2(0.0, 0), vec2(size.x-mr-1.0, width));\n"
      elseif not rc.bl and not rc.br then shader=shader.."  float botl = box(lpc, vec2(0.0, 0), vec2(size.x, width));\n" 
      end
      table.insert(parts, "botl")
    end
    
    if s.left then
      if rc.tl and rc.bl then shader=shader.."  float leftl = box(lpc, vec2(0, mr-1.0), vec2(width, size.y-mr-1.0));\n"
      elseif not rc.tl and rc.bl then shader=shader.."  float leftl = box(lpc, vec2(0, mr-1.0), vec2(width, size.y));\n"
      elseif rc.tl and not rc.bl then shader=shader.."  float leftl = box(lpc, vec2(0, 0.0), vec2(width, size.y-mr-1.0));\n"
      elseif not rc.tl and not rc.bl then shader=shader.."  float leftl = box(lpc, vec2(0, 0.0), vec2(width, size.y));\n"
      end
      table.insert(parts, "leftl")
    end

    if s.right then
      if rc.tr and rc.br then shader=shader.."  float rightl = box(lpc, vec2(size.x-width-1.0, mr-1.0), vec2(size.x-1.0, size.y-mr-1.0));\n"
      elseif not rc.tr and rc.br then shader=shader.."  float rightl = box(lpc, vec2(size.x-width-1.0, mr-1.0), vec2(size.x-1.0, size.y));\n"
      elseif rc.tr and not rc.br then shader=shader.."  float rightl = box(lpc, vec2(size.x-width-1.0, 0.0), vec2(size.x-1.0, size.y-mr-1.0));\n"
      elseif not rc.tr and not rc.br then shader=shader.."  float rightl = box(lpc, vec2(size.x-width-1.0, 0.0), vec2(size.x-1.0, size.y));\n"
      end
      table.insert(parts, "rightl")
    end
    
    -- assemble parts
    shader = shader .. "  float f = 0.0"
    for k,v in pairs(parts) do
      shader = shader .. "+" .. v
    end
    shader = shader..";\n"
    shader = shader .. "  return clamp(f, 0.0, 1.0);\n}"
    shader=shader..[[

vec2 localPixelCoord() 
{
  return tc0*(size-vec2(1,1));
}

void main(void)
{  
  gl_FragColor = color*roundedRectFrame(localPixelCoord(), size, radius, width);
}
    
]]
  end
  
  log.debug("-------------- VERTEX SHADER")
  log.debug(roundedRectVsSource)
  log.debug("-------------- FRAGMENT SHADER")
  log.debug(shader)
  
  if filled then
    result = lost.gl.buildShader(self.loader, "roundedRect", self.roundedRectVsSource, shader)
  else
    result = lost.gl.buildShader(self.loader, "roundedRectFrame", self.roundedRectFrameVsSource, shader)
  end
  
  self.cache[cacheKey] = result
  log.debug("build shader for key "..cacheKey)
  
  return result
end


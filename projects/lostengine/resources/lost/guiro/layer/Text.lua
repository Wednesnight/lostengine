module("lost.guiro.layer", package.seeall)

require("lost.common.ShaderFactory")
require("lost.guiro.layer.Layer")
require("lost.guiro.Bounds")
require("lost.guiro.Quad")

lost.common.Class "lost.guiro.layer.Text" "lost.guiro.layer.Layer" {}

local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local MatrixTranslation = lost.math.MatrixTranslation
local Color = lost.common.Color

local breakModes = {none=0, char=1, word=2}

-- valid params:
-- * all layer params
-- * font: must be a table with two entries in this order: a string containing a font name known to the fontmanager, a number containing the desired fontsize
-- * text: the string to render
function Text:constructor(args)
  lost.guiro.layer.Layer.constructor(self, args)
  local t = args or {}
  self.id = t.id or "text"

  self.buffer = lost.font.TextBuffer()

  self.mesh = lost.font.RenderedText.create()
  self.mesh.material.shader = lost.guiro.shaderFactory():texture()
  self.mesh.material:blendPremultiplied()
  
  self.shadowMesh = lost.font.RenderedText.create()
  self.shadowMesh.material.shader = lost.guiro.shaderFactory():texture()
  self.shadowMesh.material:blendNormal()
  
  self.shadowDrawNode = lost.rg.Draw.create(self.shadowMesh)
  self.renderNode:add(self.shadowDrawNode)
  self.drawNode = lost.rg.Draw.create(self.mesh)
  self.renderNode:add(self.drawNode)
  
  if t.font then self:font(t.font) end
  if t.breakMode then
    self:breakMode(t.breakMode)
  else
    self:breakMode("none")
  end
  self._dirtyText = false
  self._lastBuildWidth = 0
  self:text(t.text or " ")
  self:color(t.color or Color(0,0,0))
  self:shadow(t.shadow or false)
  self:shadowOffset(t.shadowOffset or Vec2(1,1))
  self:shadowColor(t.shadowColor or Color(0,0,0,.3))
  self:halign(t.halign or "center")
  self:valign(t.valign or "center")
  self:characterMetrics(t.characterMetrics or false)
  self:needsDisplay()
  self.alignedMeshPos = lost.math.Vec3()
  self:cursorPos(Vec2(0,0))
  self._scrollOffset = lost.math.Vec2(0,0)
end

function Text:characterMetrics(...)
  if arg.n >=1 then
    self.buffer.characterMetrics = arg[1]
  else
    return self.buffer.characterMetrics
  end
end

function Text:cursorPos(...)
  if arg.n >= 1 then
    self._cursorPos = arg[1]
    self:needsLayout()
    if self.cursorLayer then self.cursorLayer:needsLayout() end
  else
    return self._cursorPos
  end
end

function Text:updateX()
  local pl = self.buffer:numPhysicalLines()
  if self._cursorPos.y >= pl then self._cursorPos.y = pl-1 end
  if self._cursorPos.y < 0 then self._cursorPos.y = 0 end
  local nc = self.buffer:numCharsInPhysicalLine(self._cursorPos.y)
  if self._cursorPos.x > nc then
    self._cursorPos.x = nc
  end
end

function Text:updateXY()
  local pl = self.buffer:numPhysicalLines()
  if self._cursorPos.y >= pl then self._cursorPos.y = pl-1 end
  if self._cursorPos.y < 0 then self._cursorPos.y = 0 end
  local nc = self.buffer:numCharsInPhysicalLine(self._cursorPos.y)
  if self._cursorPos.x <= 0 and self._cursorPos.y > 0 then
    self._cursorPos.y = self._cursorPos.y - 1
    self:cursorLastX()
  elseif self._cursorPos.x > nc and self._cursorPos.y < pl-1 then
    self._cursorPos.y = self._cursorPos.y + 1
    self:cursorFirstX()
  end
end

function Text:cursorFirstY()
  self._cursorPos.y = 0
  self:updateX()
  self:needsLayout()
end

function Text:cursorLastY()
  self._cursorPos.y = self.buffer:numPhysicalLines()
  self:updateX()
  self:needsLayout()
end

function Text:cursorIncY(y)
  self._cursorPos.y = self._cursorPos.y + (y or 1)
  self:updateX()
  self:needsLayout()
end

function Text:cursorDecY(y)
  self._cursorPos.y = self._cursorPos.y - (y or 1)
  self:updateX()
  self:needsLayout()
end

function Text:cursorFirstX()
  self._cursorPos.x = 0
  self:needsLayout()
end

function Text:cursorLastX()
  local pl = self.buffer:numPhysicalLines()
  if self._cursorPos.y >= pl then self._cursorPos.y = pl-1 end
  if self._cursorPos.y < 0 then self._cursorPos.y = 0 end
  self._cursorPos.x = self.buffer:numCharsInPhysicalLine(self._cursorPos.y)
  self:needsLayout()
end

function Text:cursorIncX(x)
  self._cursorPos.x = self._cursorPos.x + (x or 1)
  self:updateXY()
  self:needsLayout()
end

function Text:cursorDecX(x)
  self._cursorPos.x = self._cursorPos.x - (x or 1)
  self:updateXY()
  self:needsLayout()
end

function Text:repositionMeshes(x,y) -- Vec2
  self.alignedMeshPos.x = math.floor(x)
  self.alignedMeshPos.y = math.floor(y)

  self.mesh.transform = MatrixTranslation(self.alignedMeshPos)  
  self.shadowMesh.transform = MatrixTranslation(self.alignedMeshPos+Vec3(self._shadowOffset.x, self._shadowOffset.y,0))  
end

function Text:updateAlign()
  self.alignedMeshPos = Vec3(self.rect.x, self.rect.y, 0)

  if self._halign == "center" then
    self.alignedMeshPos.x = self.rect.x + (self.rect.width-self.mesh.size.width)/2
  elseif self._halign == "left" then
    self.alignedMeshPos.x = self.rect.x
  elseif self._halign == "right" then
    self.alignedMeshPos.x = self.rect.x+self.rect.width - self.mesh.size.width
  end

  if self._valign == "center" then
      self.alignedMeshPos.y = self.rect.y+((self.rect.height-(self.buffer:numPhysicalLines()*math.floor(self._font.lineHeight)))/2)+math.abs(self._font.descender)
  elseif self._valign == "top" then
    self.alignedMeshPos.y = self.rect.y+self.rect.height-(self.buffer:numPhysicalLines()*math.floor(self._font.lineHeight))
  elseif self._valign == "bottom" then
    self.alignedMeshPos.y = self.rect.y-self._font.descender+1
  end
  
  self:repositionMeshes(self.alignedMeshPos.x, self.alignedMeshPos.y)
end

function Text:updateCursor()
  if self.cursorLayer then
    local pl = self.buffer:numPhysicalLines()

    if self._cursorPos.y >= pl then self._cursorPos.y = pl-1 end
    if self._cursorPos.y < 0 then self._cursorPos.y = 0 end

    -- clip cursor position to allowed range of current line
    local nc = self.buffer:numCharsInPhysicalLine(self._cursorPos.y) -- clip y for real y movement 
    
    if self._cursorPos.x < 0 then self._cursorPos.x = 0 end
    if self._cursorPos.x > nc then self._cursorPos.x = nc end

    local lh = self._font.lineHeight
    local x = 0
    local y = (self.alignedMeshPos.y + self.rect.height) - (self._font.descender + self._cursorPos.y * lh)
    local r = self.mesh:characterRect(self._cursorPos.y, self._cursorPos.x)

    if nc == 0 then 
      x = 0
    elseif nc > 0 and self._cursorPos.x == nc then 
      r = self.mesh:characterRect(self._cursorPos.y, nc-1)
      x = r.x+self.alignedMeshPos.x-self.rect.x+r.width
    else 
      x = r.x + self.alignedMeshPos.x - self.rect.x
    end

    if pl == 0 then
      y = 0
    else
      y = r.y + self._font.descender + self.alignedMeshPos.y - self.rect.y
    end

    if type(x) == "number" then
      local b = 2
--      log.debug("x "..tostring(x))
      if (x < b) then
--        log.debug("cursor out left")
        self._scrollOffset.x = x + b
      end
      if (x > (self.rect.width - b)) then
--        log.debug("cursor out right");
        self._scrollOffset.x = x - (self.rect.width - b)
      end
      x = math.max(x - self._scrollOffset.x, 0)
    end

    if type(y) == "number" then
      local b = 2
--      log.debug("y "..tostring(y))
      if (y < b) then
--        log.debug("cursor out bottom")
        self._scrollOffset.y = y + b
      end
      if (y > (self.rect.height - lh + b)) then
--        log.debug("cursor out top");
        self._scrollOffset.y = y - (self.rect.height - lh + b)
      end
      y = math.max(y - self._scrollOffset.y, 0)
    end

--    log.debug("self._scrollOffset "..tostring(self._scrollOffset))
--    log.debug("self.alignedMeshPos "..tostring(self.alignedMeshPos))
    self:repositionMeshes(self.alignedMeshPos.x - self._scrollOffset.x, self.alignedMeshPos.y - self._scrollOffset.y)

--    log.debug(tostring(self.rect))
--    log.debug(x ..", ".. y)
    self.cursorLayer:x(x)
    self.cursorLayer:y(y)
    self.cursorLayer:height(lh)
    self.cursorLayer:needsUpdate()
  end
end

function Text:meshNeedsRebuild()
  local widthChanged = self._lastBuildWidth ~= self.rect.width
  return self._dirtyText or ((self._breakMode ~= "none") and widthChanged)
end

function Text:updateMesh()
  if self:meshNeedsRebuild() then
    local pl = self.buffer:numPhysicalLines()

--    self.buffer:reset(self.#, self._font, breakModes[self._breakMode],self.rect.width)
    self.buffer:width(self.rect.width)
    self.buffer:reset()
--    log.debug("rebuilding text mesh")
    self.buffer:renderAllPhysicalLines(self.mesh)
    if self.shadowDrawNode.active then
      self.shadowMesh = self.mesh:clone()
      self.shadowMesh.material = self.mesh.material:clone()
      self.shadowMesh.material.color = self._shadowColor
      self.shadowDrawNode.mesh = self.shadowMesh
    end
    self._dirtyText = false
    self._lastBuildWidth = self.rect.width

    if pl < self.buffer:numPhysicalLines() then
      self:cursorLastY()
      self:cursorLastX()
    end
  end
end

function Text:updateLayout()
--  log.debug("layout")
  lost.guiro.layer.Layer.updateLayout(self)
  self:updateMesh()
  self:updateAlign()
  self:updateCursor()
end

function Text:updateDisplay()
--  log.debug("display")
  lost.guiro.layer.Layer.updateDisplay(self)
  self:updateMesh()
  self:updateAlign()
  self:updateCursor()
end

function Text:breakMode(...)
  if arg.n >= 1 then
    local bm = arg[1]
    if (bm == "none") or (bm=="char") or (bm=="word") then
      self.buffer:breakMode(breakModes[bm])
    else
      self.buffer:breakMode(breakModes["none"])
    end
    self._dirtyText = true -- trigger redraw like this even if the text hasn't changed since it needs to be rebuild anyway
    self:needsDisplay()
  else
    return nil -- self._breakMode
  end
end

function Text:text(...)
  if arg.n >= 1 then
    self.buffer:text(arg[1])
    self._dirtyText = true
    self:needsDisplay()
  else
    return self.buffer:utf8String()
  end
end

function Text:renderedTextWidth()
  return self.mesh.size.width
end

function Text:appendText(s)
--  self._text = self._text..s
--  self._dirtyText = true
--  self:needsDisplay()
end

function Text:font(...)
  if arg.n >=1 then
    self._dirtyText = true -- trigger redraw like this even if the text hasn't changed since it needs to be rebuild anyway
    self._font = tasklet.fontManager:getFont(arg[1][1], arg[1][2])
    self.buffer:font(self._font)
    self:needsDisplay()
  else
    return self._font
  end
end

function Text:color(v)
  self.mesh.material.color = v
end

function Text:valign(v)
  self._dirtyText = true -- trigger redraw like this even if the text hasn't changed since it needs to be rebuild anyway
  self._valign = v
  self:needsLayout()
end

function Text:halign(v)
  self._dirtyText = true -- trigger redraw like this even if the text hasn't changed since it needs to be rebuild anyway
  self._halign = v
  self:needsLayout()
end

function Text:shadow(...)
  if arg.n >= 1 then
    self._dirtyText = true -- trigger redraw like this even if the text hasn't changed since it needs to be rebuild anyway
    self.shadowDrawNode.active = arg[1]
    self:needsDisplay()
  else
    return self.shadowDrawNode.active
  end
end

function Text:shadowOffset(...)
  if arg.n >= 1 then
    self._dirtyText = true -- trigger redraw like this even if the text hasn't changed since it needs to be rebuild anyway
    self._shadowOffset = arg[1]
    self:needsLayout()
  else
    return self._shadowOffset
  end
end

function Text:shadowColor(...)
  if arg.n >= 1 then
    self._shadowColor = arg[1]
    self.shadowMesh.material.color = self._shadowColor
  else
    return self.shadowMesh.color
  end
end

-- returns the rect of the specified character within the text layer
-- mesh alignment is taken into account
function Text:characterRect(lineIndex, charIndex)
  local result = self.mesh:characterRect(lineIndex, charIndex)
  result.x = result.x + self.alignedMeshPos.x
  result.y = result.y + self.alignedMeshPos.y
  return result
end

function Text:insertAtCursor(utf8str)
  self.buffer:insertUtf8StringAtPosition(self._cursorPos.y, self._cursorPos.x, utf8str)
  self:cursorIncX(string.len(utf8str))
  self._dirtyText = true
  self:needsDisplay()
end

function Text:eraseBeforeCursor()
  if self._cursorPos.x > 0 or self._cursorPos.y > 0 then
--    log.debug("erasing char at pos "..(self._cursorPos.y).." "..tostring(self._cursorPos.x-1))
    self.buffer:eraseCharAtPosition(self._cursorPos.y, self._cursorPos.x-1)
    self:cursorDecX()
    self._dirtyText = true
    self:needsDisplay()
  end
end

function Text:eraseAfterCursor()
  if self._cursorPos.x ~= self.buffer:numCharsInPhysicalLine(self._cursorPos.y) or
     self._cursorPos.y < self.buffer:numPhysicalLines()
  then
--    log.debug("erasing char at pos "..(self._cursorPos.y).." "..tostring(self._cursorPos.x))
    self.buffer:eraseCharAtPosition(self._cursorPos.y, self._cursorPos.x)
    self._dirtyText = true
    self:needsDisplay()
  end
end

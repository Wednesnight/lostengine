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
  self._undoBufferSize = 50
  self._redoBufferSize = 50
  self:resetUndoRedoBuffers()
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
    local nc = self.buffer:numCharsInPhysicalLine(self._cursorPos.y)
    
    if self._cursorPos.x < 0 then self._cursorPos.x = 0 end
    if self._cursorPos.x > nc then self._cursorPos.x = nc end

    local lh = math.floor(self._font.lineHeight)
    local r = self.mesh:characterRect(self._cursorPos.y, self._cursorPos.x)
    local x = r.x + self.alignedMeshPos.x - self.rect.x
    local y = (((self.alignedMeshPos.y - self.rect.y) + pl * lh) - (self._cursorPos.y + 1) * lh) + self._font.descender + 1

    if nc == 0 then 
      x = self.alignedMeshPos.x - self.rect.x
    elseif nc > 0 and self._cursorPos.x == nc then 
      r = self.mesh:characterRect(self._cursorPos.y, nc - 1)
      x = r.x + self.alignedMeshPos.x - self.rect.x + r.width
    end

    if type(x) == "number" then
      local lx = x - self._scrollOffset.x
      if (lx < 0) then
        self._scrollOffset.x = x
      end
      if lx > self.rect.width then
        self._scrollOffset.x = x - self.rect.width
      end
      x = x - self._scrollOffset.x
    end

    if pl == 0 then
      y = self._valign
    end

    if type(y) == "number" then
      local ly = y - self._scrollOffset.y
      if (ly < 0) then
        log.debug("out bottom: ".. ly)
        self._scrollOffset.y = y
      end
      if ly > self.rect.height - lh then
        log.debug("out top: ".. ly)
        self._scrollOffset.y = y - (self.rect.height - lh)
      end
      y = y - self._scrollOffset.y
    end

    self:repositionMeshes(self.alignedMeshPos.x - self._scrollOffset.x, self.alignedMeshPos.y - self._scrollOffset.y)

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
  lost.guiro.layer.Layer.updateLayout(self)
  self:updateMesh()
  self:updateAlign()
  self:updateCursor()
end

function Text:updateDisplay()
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
    self:resetUndoRedoBuffers()
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
  self:addUndoRedo(function()
    self.buffer:insertUtf8StringAtPosition(self._cursorPos.y, self._cursorPos.x, utf8str)
    self:cursorIncX(string.len(utf8str))
    self._dirtyText = true
    self:needsDisplay()
  end)
end

function Text:eraseBeforeCursor()
  if self._cursorPos.x > 0 or self._cursorPos.y > 0 then
    self:addUndoRedo(function()
      self:cursorDecX()
      self.buffer:eraseCharAtPosition(self._cursorPos.y, self._cursorPos.x)
      self._dirtyText = true
      self:needsDisplay()
    end)
  end
end

function Text:eraseAfterCursor()
  if self._cursorPos.x ~= self.buffer:numCharsInPhysicalLine(self._cursorPos.y) or
     self._cursorPos.y < self.buffer:numPhysicalLines()
  then
    self:addUndoRedo(function()
      self.buffer:eraseCharAtPosition(self._cursorPos.y, self._cursorPos.x)
      self._dirtyText = true
      self:needsDisplay()
    end)
  end
end

function Text:selectionRect(cx, cy)
  local pl = self.buffer:numPhysicalLines()

  if cy >= pl then cy = pl-1 end
  if cy < 0 then cy = 0 end

  local nc = self.buffer:numCharsInPhysicalLine(cy)
  
  if cx < 0 then cx = 0 end
  if cx > nc then cx = nc end


  local r = nil
  if nc > 0 and cx == nc then 
    r = self.mesh:characterRect(cy, nc - 1)
    r.x = r.x + (self.alignedMeshPos.x - self.rect.x) + r.width
  else
    r = self.mesh:characterRect(cy, cx)
    r.x = r.x + (self.alignedMeshPos.x - self.rect.x)
  end

  local lh = math.floor(self._font.lineHeight)
  return lost.math.Rect(
    math.min(math.max(r.x, 0), self.rect.width),
    math.min(math.max((((self.alignedMeshPos.y - self.rect.y) + pl * lh) - (cy + 1) * lh) + self._font.descender + 1, 0), self.rect.height),
    0,
    lh
  )
end

function Text:windowToCursorCoords(windowCoords)
  local cx, cy = self._cursorPos.x, self._cursorPos.y
  if windowCoords.x >= self.alignedMeshPos.x and windowCoords.x < self.rect.x + self.rect.width and
     windowCoords.y >= self.alignedMeshPos.y and windowCoords.y < self.rect.y + self.rect.height
  then
    local x = windowCoords.x - self.alignedMeshPos.x
    local y = windowCoords.y - self.alignedMeshPos.y
    local pl = self.buffer:numPhysicalLines()
    local lh = math.floor(self._font.lineHeight)
    cy = pl - math.ceil(y / lh)
    if cy >= pl then cy = pl-1 end
    if cy < 0 then cy = 0 end
    local nc = self.buffer:numCharsInPhysicalLine(cy)
    cx = nc
    while cx > 0 do
      cx = cx - 1
      local r = self.mesh:characterRect(cy, cx)
      if r.x <= x then
        if r.x + r.width <= x then
          cx = cx + 1
        end
        break
      end
    end
  end
  return cx, cy
end

function Text:moveCursor(windowCoords)
  self._cursorPos.x, self._cursorPos.y = self:windowToCursorCoords(windowCoords)
  self:needsLayout()
end

function Text:hasSelection()
  return self._selection ~= nil
end

function Text:getSelectionPos()
  return self._selection.from.x, self._selection.from.y, self._selection.to.x, self._selection.to.y
end

function Text:getSelectionRange()
  if self._selection.from.y < self._selection.to.y or (self._selection.from.y == self._selection.to.y and self._selection.from.x < self._selection.to.x) then
    return self._selection.from.x, self._selection.from.y, self._selection.to.x, self._selection.to.y
  else
    return self._selection.to.x, self._selection.to.y, self._selection.from.x, self._selection.from.y
  end
end

function Text:getSelection()
  if self:hasSelection() then
    local fx, fy, tx, ty = self:getSelectionRange()
    return self.buffer:substring(fy, fx, ty, tx)
  end
  return nil
end

function Text:eraseSelection()
  if self:hasSelection() then
    self:addUndoRedo(function()
      local fx, fy, tx, ty = self:getSelectionRange()
      self.buffer:eraseChars(fy, fx, ty, tx)
      self._cursorPos.x, self._cursorPos.y = fx, fy
      self:needsLayout()
      self:clearSelection()
      self._dirtyText = true
      self:needsDisplay()
    end)
  end
  return nil
end

-- set by cursor pos
function Text:setSelection(fx, fy, tx, ty)
  if self.selectionLayers and (math.abs(tx - fx) > 0 or math.abs(ty - fy) > 0) then

    self._selection = {from = {x = fx, y = fy}, to = {x = tx, y = ty}}

    --[[

         [...]    <-- upper
      [......]    <-- middle
      [......]    <-- middle
      [...]       <-- lower

      ]]
    local upper = nil
    local lower = nil
    local middle = nil

    if fy < ty or (fy == ty and fx < tx) then
      upper = {x = fx, y = fy}
    else
      upper = {x = tx, y = ty}
    end

    local delta = math.abs(ty - fy)

    if delta > 0 then
      if fy >= ty then
        lower = {x = fx, y = fy}
      else
        lower = {x = tx, y = ty}
      end
      if delta > 1 then
        middle = {x = 0, y = lower.y-1}
      end
    end

    if upper then
      local r = self:selectionRect(upper.x, upper.y)
      if upper.x <= 0 then
        r.x = 0
      end
      if delta > 0 then
        r.width = self.rect.width - r.x
      else
        local lr = nil
        if fy < ty or (fy == ty and fx < tx) then
          lr = self:selectionRect(tx, ty)
        else
          lr = self:selectionRect(fx, fy)
        end
        r.width = lr.x - r.x
      end
      self.selectionLayers[1]:pos(r.x, r.y)
      self.selectionLayers[1]:size(r.width, r.height)
      self.selectionLayers[1]:needsUpdate()
      self.selectionLayers[1]:hidden(false)
    else
      self.selectionLayers[1]:hidden(true)
    end

    if lower then
      local r = self:selectionRect(lower.x, lower.y)
      if lower.x <= 0 then
        r.x = 0
      end
      r.width = r.x
      self.selectionLayers[2]:pos(0, r.y)
      self.selectionLayers[2]:size(r.width, r.height)
      self.selectionLayers[2]:needsUpdate()
      self.selectionLayers[2]:hidden(false)
    else
      self.selectionLayers[2]:hidden(true)
    end

    if middle then
      local r = self:selectionRect(middle.x, middle.y)
      self.selectionLayers[3]:pos(0, r.y)
      self.selectionLayers[3]:size(self.rect.width, r.height * (delta-1))
      self.selectionLayers[3]:needsUpdate()
      self.selectionLayers[3]:hidden(false)
    else
      self.selectionLayers[3]:hidden(true)
    end

    local dummy = nil
    dummy, dummy, self._cursorPos.x, self._cursorPos.y = self:getSelectionPos()
    self:needsLayout()
  else
    self:clearSelection()
  end
end

-- set by window coordinates
function Text:setSelectionCoords(from, to)
  local fx, fy = self:windowToCursorCoords(from)
  local tx, ty = self:windowToCursorCoords(to)
  self:setSelection(fx, fy, tx, ty)
end

function Text:clearSelection()
  if self._selection ~= nil then
  end
  self._selection = nil
  if self.selectionLayers then
    for k,s in next,self.selectionLayers do
      s:hidden(true)
    end
  end
end

function Text:selectAll()
  local maxY = self.buffer:numPhysicalLines()-1
  local maxX = self.buffer:numCharsInPhysicalLine(maxY)
  self:setSelection(0, 0, maxX, maxY)
end

function Text:getCursorPos()
  return self._cursorPos.x, self._cursorPos.y
end

function Text:resetUndoRedoBuffers()
  self:resetUndoBuffer()
  self:resetRedoBuffer()
end

function Text:resetUndoBuffer()
  self._undoBuffer = {}
  self._undoBufferPos = self._undoBufferSize
end

function Text:resetRedoBuffer()
  self._redoBuffer = {}
  self._redoBufferPos = self._redoBufferSize
end

function Text:addUndoRedo(f)
  local before = self.buffer:utf8String()
  local bx, by = self._cursorPos.x, self._cursorPos.y
  f()
  local after = self.buffer:utf8String()
  local ax, ay = self._cursorPos.x, self._cursorPos.y
  local undo
  undo = function()
    self._cursorPos.x, self._cursorPos.y = bx, by
    self:needsLayout()
    self.buffer:text(before)
    self._dirtyText = true
    self:needsDisplay()
    self:addRedo(function()
      self._cursorPos.x, self._cursorPos.y = ax, ay
      self:needsLayout()
      self.buffer:text(after)
      self._dirtyText = true
      self:needsDisplay()
      self:addUndo(undo, false)
    end)
  end
  self:addUndo(undo)
end

function Text:addUndo(u, reset)
  self._undoBuffer[self._undoBufferPos] = u
  self._undoBufferPos = self._undoBufferPos - 1
  if self._undoBufferPos <= 0 then
    self._undoBufferPos = self._undoBufferSize
  end
  if reset ~= false then
    self:resetRedoBuffer()
  end
end

function Text:addRedo(r)
  self._redoBuffer[self._redoBufferPos] = r
  self._redoBufferPos = self._redoBufferPos - 1
  if self._redoBufferPos <= 0 then
    self._redoBufferPos = self._redoBufferSize
  end
end

function Text:undo()
  if table.maxn(self._undoBuffer) > 0 then
    self._undoBufferPos = self._undoBufferPos + 1
    if self._undoBufferPos > self._undoBufferSize then
      self._undoBufferPos = 1
    end
    self._undoBuffer[self._undoBufferPos]()
    self._undoBuffer[self._undoBufferPos] = nil
  end
end

function Text:redo()
  if table.maxn(self._redoBuffer) > 0 then
    self._redoBufferPos = self._redoBufferPos + 1
    if self._redoBufferPos > self._redoBufferSize then
      self._redoBufferPos = 1
    end
    self._redoBuffer[self._redoBufferPos]()
    self._redoBuffer[self._redoBufferPos] = nil
  end
end


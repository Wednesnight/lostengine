module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.TextInput" "lost.guiro.view.View" {}

function TextInput:constructor(args)
	lost.guiro.view.View.constructor(self, args) -- call after all members that are required for style are setup
  self.id = args.id or "textinput"
  self.focusable = true
  self.focusLayer:hidden(true)
  self:addEventListener("focusReceived", function(event) self:focusReceived(event) end)
  self:addEventListener("focusLost", function(event) self:focusLost(event) end)
  self:addEventListener("keyDown", function(event) self:keyDown(event) end)
  self:addEventListener("mouseDown", function(event) self:mouseDown(event) end)
  self:addEventListener("mouseMove", function(event) self:mouseMove(event) end)
  self:addEventListener("mouseUp", function(event) self:mouseUp(event) end)
  if args.font then self.textLayer:font(args.font) end
  self.textLayer:text(args.text or "")
  self:valign(args.valign or "center")
  self:halign(args.halign or "left")
  self:lineBreak(args.lineBreak or "\n")  -- line breaks = \n
  self:tabSize(args.tabSize or 4)         -- tab = 4 spaces
  self:multiLine(args.multiLine or false)
  self:needsDisplay()
  self:needsLayout()
  self.textLayer:cursorPos(lost.math.Vec2(0,0))
  self.cursorLayer:hidden(true)
  self.cursorTimer = tasklet.scheduler:createTimer(.5, self.toggleTimer, self)
end

function TextInput:multiLine(v)
  self._multiLine = v
end

function TextInput:lineBreak(v)
  self._lineBreak = v
end

function TextInput:tabSize(v)
  self._tabSize = v
end

function TextInput:valign(v)
  self.textLayer:valign(v)
end

function TextInput:halign(v)
  self.textLayer:halign(v)
end

function TextInput:toggleTimer(timer)
  self.cursorLayer:hidden(not self.cursorLayer:hidden())
  return true -- continue blinking
end

function TextInput:focusReceived(event)
  self.focusLayer:hidden(false)
  self.cursorLayer:hidden(false)
  self.cursorTimer:start()  -- start blinking
end

function TextInput:focusLost(event)
  self.focusLayer:hidden(true)
  self.cursorLayer:hidden(true)
  self.cursorTimer:stop() -- stop blinking
end

function TextInput:text(...)
  if arg.n >= 1 then
    self.textLayer:text(arg[1])
  else
    return self.textLayer:text()
  end
end

function TextInput:updateSelection(x, y)
  local tx, ty = self.textLayer:getCursorPos()
  self.textLayer:setSelection(x, y, tx, ty)
end

function TextInput:keyDown(event)
  local selX, selY
  if event.shiftDown then
    if self.textLayer:hasSelection() then
      selX, selY = self.textLayer:getSelectionPos()
    else
      selX, selY = self.textLayer:getCursorPos()
    end
  end

  if event.key == lost.application.K_RIGHT then
    self.textLayer:cursorIncX()
    if event.shiftDown then
      self:updateSelection(selX, selY)
    else
      self.textLayer:clearSelection()
    end
  elseif event.key == lost.application.K_LEFT then
    self.textLayer:cursorDecX()
    if event.shiftDown then
      self:updateSelection(selX, selY)
    else
      self.textLayer:clearSelection()
    end
  elseif self._multiLine and event.key == lost.application.K_UP and self._multiLine then
    self.textLayer:cursorDecY()
    if event.shiftDown then
      self:updateSelection(selX, selY)
    else
      self.textLayer:clearSelection()
    end
  elseif self._multiLine and event.key == lost.application.K_DOWN and self._multiLine then
    self.textLayer:cursorIncY()
    if event.shiftDown then
      self:updateSelection(selX, selY)
    else
      self.textLayer:clearSelection()
    end
  elseif event.key == lost.application.K_HOME then
    if self._multiLine and (event.ctrlDown or event.specialDown) then
      self.textLayer:cursorFirstY()
      self.textLayer:cursorFirstX()
    else
      self.textLayer:cursorFirstX()
    end
    if event.shiftDown then
      self:updateSelection(selX, selY)
    else
      self.textLayer:clearSelection()
    end
  elseif event.key == lost.application.K_END then
    if self._multiLine and (event.ctrlDown or event.specialDown) then
      self.textLayer:cursorLastY()
      self.textLayer:cursorLastX()
    else
      self.textLayer:cursorLastX()
    end
    if event.shiftDown then
      self:updateSelection(selX, selY)
    else
      self.textLayer:clearSelection()
    end
  elseif event.key == lost.application.K_ENTER then
    if self._multiLine then
      if self.textLayer:hasSelection() then
        self.textLayer:eraseSelection()
      end
      self.textLayer:insertAtCursor(self._lineBreak)
      self:dispatchInputEvent("valueChanged")
    else
      self:dispatchInputEvent("enterPressed")
    end
  elseif event.key == lost.application.K_TAB then
    if self.textLayer:hasSelection() then
      self.textLayer:eraseSelection()
    end
    local tab = ""
    local tabSize = math.max(self._tabSize, 0)
    for k = 1, tabSize do
      tab = tab .." "
    end
    self.textLayer:insertAtCursor(tab)
    self:dispatchInputEvent("valueChanged")
  elseif event.key == lost.application.K_ESCAPE then 
  elseif event.key == lost.application.K_BACKSPACE then 
    if self.textLayer:hasSelection() then
      self.textLayer:eraseSelection()
    else
      self.textLayer:eraseBeforeCursor()
    end
    self:dispatchInputEvent("valueChanged")
  elseif event.key == lost.application.K_DELETE then
    if self.textLayer:hasSelection() then
      self.textLayer:eraseSelection()
    else
      self.textLayer:eraseAfterCursor()
    end
    self:dispatchInputEvent("valueChanged")
  elseif event.key == lost.application.K_Z and not event.shiftDown and (event.ctrlDown or event.specialDown) then
    self.textLayer:undo()
  elseif ((event.key == lost.application.K_Y) or (event.key == lost.application.K_Z and event.shiftDown)) and (event.ctrlDown or event.specialDown) then
    self.textLayer:redo()
  elseif (event.key == lost.application.K_C or event.key == lost.application.K_X) and (event.ctrlDown or event.specialDown) then
    local str = self.textLayer:getSelection()
    if str ~= nil and string.len(str) > 0 then
      tasklet:setClipboardString(str)
      if event.key == lost.application.K_X then
        self.textLayer:eraseSelection()
      end
    end
  elseif event.key == lost.application.K_V and (event.ctrlDown or event.specialDown) then
    local str = tasklet:getClipboardString()
    if string.len(str) > 0 then
      if self.textLayer:hasSelection() then
        self.textLayer:eraseSelection()
      end
      self.textLayer:insertAtCursor(str)
      self:dispatchInputEvent("valueChanged")
    end
  elseif event.key == lost.application.K_A and (event.ctrlDown or event.specialDown) then
    self.textLayer:selectAll()
  elseif string.len(event.character) > 0 and (not event.ctrlDown and not event.specialDown and not event.altDown) then
    if self.textLayer:hasSelection() then
      self.textLayer:eraseSelection()
    end
    self.textLayer:insertAtCursor(event.character)
    self:dispatchInputEvent("valueChanged")
  end

  -- only blink when idle
  self.cursorLayer:hidden(false)
  self.cursorTimer:restart()
end

function TextInput:dispatchInputEvent(name)
  local event = lost.guiro.event.Event(name)
  event.bubbles = true
  event.target = self
  self:dispatchEvent(event)  
end

function TextInput:mouseDown(event)
  self.textLayer:clearSelection()
  self._selectionStart = event.pos
  self.textLayer:moveCursor(event.pos)
end

function TextInput:mouseMove(event)
  if event.pressed and self._selectionStart then
    self._selecting = true
    self.textLayer:setSelectionCoords(self._selectionStart, event.pos)
    -- only blink when idle
    self.cursorLayer:hidden(true)
    self.cursorTimer:restart()
  end
end

function TextInput:mouseUp(event)
  if self._selecting then
    self.textLayer:setSelectionCoords(self._selectionStart, event.pos)
    self.textLayer:moveCursor(event.pos)
    self._selecting = false
    self._selectionStart = nil
  end
end


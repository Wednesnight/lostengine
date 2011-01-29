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
  if args.font then self.textLayer:font(args.font) end
  self.textLayer:text(args.text or "")
  self:valign(args.valign or "center")
  self:halign(args.halign or "left")  
  self:needsDisplay()
  self:needsLayout()
  self.textLayer:cursorPos(lost.math.Vec2(0,0))
  self.cursorLayer:hidden(true)
end

function TextInput:valign(v)
  self.textLayer:valign(v)
end

function TextInput:halign(v)
  self.textLayer:halign(v)
end

function TextInput:focusReceived(event)
  self.focusLayer:hidden(false)
  self.cursorLayer:hidden(false)
end

function TextInput:focusLost(event)
  self.focusLayer:hidden(true)
  self.cursorLayer:hidden(true)
end

function TextInput:text(...)
  if arg.n >= 1 then
    self.textLayer:text(arg[1])
  else
    return self.textLayer:text()
  end
end

function TextInput:keyDown(event)
  if event.key == lost.application.K_RIGHT then self.textLayer:cursorIncX() 
  elseif event.key == lost.application.K_LEFT then self.textLayer:cursorDecX()
  elseif event.key == lost.application.K_UP then 
  elseif event.key == lost.application.K_DOWN then 
  elseif event.key == lost.application.K_ENTER then 
  elseif event.key == lost.application.K_TAB then 
  elseif event.key == lost.application.K_ESCAPE then 
  elseif event.key == lost.application.K_BACKSPACE then 
    self.textLayer:eraseBeforeCursor()
    self.textLayer:cursorDecX()
  elseif event.key == lost.application.K_DELETE then
    self.textLayer:eraseAfterCursor()
  elseif string.len(event.character) > 0 then
    self.textLayer:insertAtCursor(event.character)
    self.textLayer:cursorIncX()
  end
end


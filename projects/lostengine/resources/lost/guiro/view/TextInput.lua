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
  self.textLayer:text(args.text or "")
end

function TextInput:focusReceived(event)
  self.focusLayer:hidden(false)
end

function TextInput:focusLost(event)
  self.focusLayer:hidden(true)
end

function TextInput:text(...)
  if arg.n >= 1 then
    self.textLayer:text(arg[1])
  else
    return self.textLayer:text()
  end
end

function TextInput:keyDown(event)
  self.textLayer:appendText(event.character)
  self:needsDisplay()
end

function TextInput:updateDisplay()
  lost.guiro.view.View.updateDisplay(self)
  local r = self.textLayer:characterRect(0,0) -- r is now global within OS window context
  self.cursorLayer:x(r.x-self.textLayer.rect.x) -- which is why we need to subtract our own global x position
  log.debug("0 0 cursor pos would be "..tostring(r))
end
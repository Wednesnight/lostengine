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
  self:valign(args.valign or "center")
  self:halign(args.halign or "left")
  
  self.insertPos = 0
  self:needsDisplay()
  self:needsLayout()
end

function TextInput:valign(v)
  self.textLayer:valign(v)
end

function TextInput:halign(v)
  self.textLayer:halign(v)
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
  self.insertPos = self.insertPos + 1
  self.textLayer:cursorPos(lost.math.Vec2(self.insertPos-1,0))
  self.textLayer:appendText(event.character)
end


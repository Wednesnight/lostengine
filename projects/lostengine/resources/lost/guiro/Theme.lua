-- lost.guiro.Theme
module("lost.guiro", package.seeall) 

require("lost.common.Class")

lost.common.Class "lost.guiro.Theme" {}

function Theme:constructor(loader)
  self.name = ""
  self.styles = {}
  self.styleFuncs = {}
end

function Theme:addStyle(style)
  style.theme = self
  if not self.styles[style.targetClassName] then
    self.styles[style.targetClassName] = {}
  end
  self.styles[style.targetClassName][style.name] = style
end

function Theme:addStyleFunc(className, styleName, func)
  
end

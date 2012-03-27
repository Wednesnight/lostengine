module("lost.guiro", package.seeall) 

require("lost.common.Class")

lost.common.Class "lost.guiro.Theme" {}

function Theme:constructor()
  self.name = ""
  self.styles = {}
end

function Theme:addStyle(className, styleName, func)
  if not self.styles[className] then
    self.styles[className] = {}
  end
  self.styles[className][styleName] = func
end

function Theme:apply(target, styleName, args)
  local className = target:className()
--  log.debug("-- "..className.." -> "..styleName)
  if not self.styles[className] then
    log.warn("couldn't find styles for class '"..className.."'")
    return
  end
  local styleFunc = self.styles[className][styleName]
  if not styleFunc then
    log.warn("couldn't find style '"..styleName.."' for class '"..className.."'")
  else
    styleFunc(target, args)
  end
end
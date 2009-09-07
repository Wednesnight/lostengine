-- lost.guiro.ThemeManager
module("lost.guiro", package.seeall)

require("lost.common.Class")
require("lost.guiro.themes.default.Theme")

lost.common.Class "lost.guiro.ThemeManager" {}

function ThemeManager:constructor(loader)
  self.themes = {}
  self.defaultTheme = "default"
  self.defaultStyle = "default"
  self:addTheme(lost.guiro.themes.default.Theme(loader))
end

function ThemeManager:addTheme(theme)
  theme.themeManager = self
  self.themes[theme.name] = theme
end

function ThemeManager:apply(target, themeName, styleName)
--  log.debug("apply theme '"..themeName.."' style '"..styleName.."' to instance of "..target:className())
  
  -- FIXME: this will fail horribly if target is not an instance of a class declared with lost.common.Class.
  -- we should probably add more error checks here
  local targetClassName = target:className()
  
  local currentTheme = self.themes[themeName]
  if not currentTheme then
    log.warn("--- couldn't find theme '"..themeName.."'")
    return
  end
  
  local classStyles = currentTheme.styles[targetClassName]
  if not classStyles then
    log.warn("--- couldn't find styles for class '"..targetClassName.."' in theme '"..themeName.."'")
    return 
  end
  
  local style = classStyles[styleName]
  if not style then 
    log.warn("--- couldn't find style named '"..tostring(styleName).."' in theme '"..themeName.."' for class '"..targetClassName.."'")
    return
  end
  
  style:apply(target)
end
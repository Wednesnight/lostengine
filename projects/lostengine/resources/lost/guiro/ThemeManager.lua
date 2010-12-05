module("lost.guiro", package.seeall)

require("lost.common.Class")

lost.common.Class "lost.guiro.ThemeManager" {}

require("lost.guiro.themes.Pebble")

function ThemeManager:constructor(loader)
  self.themes = {}
  self.defaultTheme = "pebble"
  self.defaultStyle = "default"
  self:addTheme(lost.guiro.themes.Pebble())
end

function ThemeManager:addTheme(theme)
--  log.debug("/// adding theme: "..theme.name)
  self.themes[theme.name] = theme
end

function ThemeManager:apply(target, themeName, styleName, args)
    
  local theme = self.themes[themeName]
  if not theme then
    log.warn("--- couldn't find theme '"..theme.."'")
    return
  end
--  log.debug("apply theme '"..themeName.."' style '"..styleName.."' to instance of "..target:className())
  theme:apply(target, styleName, args)
end
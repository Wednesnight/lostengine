module("lost.guiro", package.seeall)

require("lost.guiro.ThemeManager")
require("lost.guiro.TextureManager")

local _textureManager = nil
local _themeManager = nil
-- due to a cyclic dependency, ui() had to be moved to lost.guiro.view.UserInterface, but you can still use lost.guiro.ui() to access it

function textureManager()
  if not _textureManager then
    _textureManager = lost.guiro.TextureManager(256)
  end
  return _textureManager
end

function themeManager()
  if not _themeManager then
    _themeManager = lost.guiro.ThemeManager(tasklet.loader)
  end
  return _themeManager
end

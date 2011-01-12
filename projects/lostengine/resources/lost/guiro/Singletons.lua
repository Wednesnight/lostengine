local _windowManager = nil

lost.guiro.windowManager = function()
  if not _windowManager then
    _windowManager = lost.guiro.WindowManager()
  end
  return _windowManager
end

local _textureManager = nil

lost.guiro.textureManager = function()
  if not _textureManager then
    _textureManager = lost.guiro.TextureManager(256)
  end
  return _textureManager
end

local _themeManager = nil

function themeManager()
  if not _themeManager then
    _themeManager = lost.guiro.ThemeManager(tasklet.loader)
  end
  return _themeManager
end

local _updateManager = nil

lost.guiro.updateManager = function()
  if not _updateManager then
    _updateManager = lost.guiro.UpdateManager()
  end
  return _updateManager
end

local _ui = nil

lost.guiro.ui = function()
  if not _ui then
    _ui = lost.guiro.view.UserInterface{}
  end
  return _ui
end



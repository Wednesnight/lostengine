module("lost.guiro", package.seeall)

require("lost.common.ShaderFactory")
require("lost.common.MeshFactory")
require("lost.guiro.ThemeManager")
require("lost.guiro.UpdateManager")
require("lost.guiro.TextureManager")

local _textureManager = nil
local _shaderFactory = nil
local _meshFactory = nil
local _updateManager = nil
local _themeManager = nil
-- due to a cyclic dependency, ui() had to be moved to lost.guiro.view.UserInterface, but you can still use lost.guiro.ui() to access it

function textureManager()
  if not _textureManager then
    _textureManager = lost.guiro.TextureManager(256)
  end
  return _textureManager
end

function shaderFactory()
  if not _shaderFactory then
    _shaderFactory = lost.common.ShaderFactory(tasklet.loader)
  end
  return _shaderFactory
end

function meshFactory()
  if not _meshFactory then
    _meshFactory = lost.common.MeshFactory(shaderFactory())
  end
  return _meshFactory
end

function updateManager()
  if not _updateManager then
    _updateManager = lost.guiro.UpdateManager()
  end
  return _updateManager
end

function themeManager()
  if not _themeManager then
    _themeManager = lost.guiro.ThemeManager(tasklet.loader)
  end
  return _themeManager
end

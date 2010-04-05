-- lost.declarative.Contex
module("lost.declarative", package.seeall)

require("lost.common.Class")
require("lost.declarative.Rg")
require("lost.declarative.Font")
require("lost.declarative.Mesh")
require("lost.declarative.Gl")
require("lost.declarative.Resource")
require("lost.declarative.Guiro")
require("lost.declarative.Bitmap")
require("lost.guiro.TextureManager")
lost.common.Class "lost.declarative.Context"
{
}

function Context:constructor(loader, textureManager)
  if not textureManager then
    self.textureManager = lost.guiro.TextureManager()
  else
    self.textureManager = textureManager
  end
  self.loader = loader
  self.rg = lost.declarative.Rg(self.loader)
  self.font = lost.declarative.Font(self.loader)
  self.mesh = lost.declarative.Mesh(self.loader)
  self.gl = lost.declarative.Gl(self.loader)
  self.resource = lost.declarative.Resource(self.loader)
  self.guiro = lost.declarative.Guiro(self.loader, lost.guiro.ThemeManager(loader), textureManager)
  self.bitmap = lost.declarative.Bitmap(self.loader)
end

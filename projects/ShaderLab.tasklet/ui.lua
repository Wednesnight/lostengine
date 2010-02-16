using "lost.guiro.Bounds"
using "lost.guiro.xabs"
using "lost.guiro.yabs"
using "lost.guiro.wrel"
using "lost.guiro.hrel"
using "lost.common.Color"
using "lost.resource.Loader"
using "lost.resource.FilesystemRepository"
using "lost.resource.ApplicationResourceRepository"
using "lost.application.K_ESCAPE"

local shaderLoader = Loader.create()
shaderLoader:addRepository(FilesystemRepository.create("/"))
shaderLoader:addRepository(ApplicationResourceRepository.create())

return dcl.guiro:Screen
{
  listeners =
  {
    keyDown = function(event)
      if event.key == K_ESCAPE then
        running = false
      end
    end,

    droppedFile = function(event)
      local self = event.target:rootView()
      self("ui")("window")("label"):text(event.filename)
    end
  },
  dcl.guiro:UserInterface
  {
    id = "ui",
    bounds = Bounds(xabs(0), yabs(0), wrel(1), hrel(1)),
    dcl.guiro:Window
    {
      id = "window",
      bounds = Bounds(xabs(0), yabs(0), wrel(1), hrel(1)),
      showFrame = false,
      showBackground = true,
      backgroundColor = Color(.1, .1, .1, 1),
      dcl.guiro:Label
      {
        id = "label",
        bounds = Bounds(xabs(0), yabs(0), wrel(1), hrel(1)),
        text = "Drop file to get started!"
      }
    }
  }
}

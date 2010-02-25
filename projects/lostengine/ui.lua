using "lost.guiro.Bounds"
using "lost.guiro.xleft"
using "lost.guiro.ybottom"
using "lost.guiro.wfit"
using "lost.guiro.hfit"
using "lost.resource.Loader"
using "lost.resource.FilesystemRepository"
using "lost.resource.ApplicationResourceRepository"
using "lost.application.SpawnTaskletEvent"
using "lost.application.currentTasklet"

return dcl.guiro:Screen
{
  listeners = 
  {
    bubble =
    {
      drop = function(event)
        local loader = Loader.create()
        loader:addRepository(FilesystemRepository.create(event.filename))
        loader:addRepository(ApplicationResourceRepository.create())
        currentTasklet:dispatchApplicationEvent(SpawnTaskletEvent.create(loader))
      end
    }
  },

  id = "screen",
  dcl.guiro:UserInterface
  {
    id = "ui",
    bounds = Bounds(xleft(), ybottom(), wfit(), hfit()),
    dcl.guiro:Window
    {
      id = "window",
      bounds = Bounds(xleft(), ybottom(), wfit(), hfit()),
      dcl.guiro:Label
      {
        id = "startupLabel",
        bounds = Bounds(xleft(), ybottom(), wfit(), hfit()),
        text = "Drop tasklet directory to get started!"
      }
    }
  }
}

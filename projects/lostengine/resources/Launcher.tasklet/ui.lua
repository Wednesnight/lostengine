using "lost.resource.Loader"
using "lost.resource.FilesystemRepository"
using "lost.resource.ApplicationResourceRepository"
using "lost.application.SpawnTaskletEvent"
using "lost.application.currentTasklet"
using "lost.common.Color"

return dcl.guiro:Screen
{
  listeners = 
  {
    bubble =
    {
      drop = function(event)
        currentTasklet:dispatchApplicationEvent(SpawnTaskletEvent.create(event.filename))
      end
    }
  },

  id = "screen",
  dcl.guiro:UserInterface
  {
    id = "ui",
    bounds = {"left", "bottom", "1", "1"},
    dcl.guiro:Window
    {
      id = "window",
      bounds = {"left", "bottom", "1", "1"}, 
      showFrame = true,
      dcl.guiro:Label
      {
        hidden = false,
        id = "startupLabel",
        bounds = {"left", "bottom", "1", "1"},
        text = "Drop tasklet directory to get started!",
      }
    }
  }
}

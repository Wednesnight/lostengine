using "lost.application.SpawnTaskletEvent"
using "lost.common.Color"

return dcl.guiro:UserInterface
{
  listeners = 
  {
    drop = function(event)
      tasklet:dispatchApplicationEvent(SpawnTaskletEvent.create(event.filename))
    end
  },
  dcl.guiro:Label
  {
    hidden = false,
    id = "startupLabel",
    bounds = {"left", "bottom", "1", "1"},
    text = "Drop tasklet directory to get started!",
  }
}

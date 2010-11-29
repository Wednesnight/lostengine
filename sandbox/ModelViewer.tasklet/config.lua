local taskletName = "Model Viewer"

return {
  tasklet = {
    name = taskletName,
    waitForEvents = true
  },
  window = {
    title = taskletName,
    x = 200,
    y = 200,
    width = 640,
    height = 480
  }
}
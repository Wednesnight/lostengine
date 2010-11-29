local screenWidth = 640
local screenHeight = 480

return {
  tasklet = {
    name = "Sphere",
    waitForEvents = false
  },
  window = {
    title = "Sphere",
    x = 50,
    y = 50,
    width = screenWidth,
    height = screenHeight
  },
  screenWidth = screenWidth,
  screenHeight = screenHeight
}
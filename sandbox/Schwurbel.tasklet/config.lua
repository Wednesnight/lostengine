local imageWidth = 400
local imageHeight = 392
local screenWidth = imageWidth*2
local screenHeight = imageHeight

return {
  tasklet = {
    name = "Schwurbel",
    waitForEvents = true
  },
  window = {
    title = "Schwurbel",
    x = 200,
    y = 200,
    width = screenWidth,
    height = screenHeight,
  },
  imageWidth = imageWidth,
  imageHeight = imageHeight,
  screenWidth = screenWidth,
  screenHeight = screenHeight,
}
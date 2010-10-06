return {
  tasklet = {
    name = "Aqua",
    waitForEvents = false
  },
  window = {
    title = "Aqua",
    x = 200,
    y = 200,
    width = 640,
    height = 480
  },
  physics = {
    gravity = lost.math.Vec2(0, -20),
    worldRatio = 1.0 / 20.0
  },
  player = {
    groundSpeed = lost.math.Vec2(10, 16),
    airSpeed = lost.math.Vec2(6, 16),
    mass = 80.0
  },
  renderer = {
    framerate = 1.0 / 30.0
  }
}

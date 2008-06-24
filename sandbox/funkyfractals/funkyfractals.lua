lsystem = LSystem()
state   = LSystemState("FzFzFzF", {F = "FzXFZxFzXFzXF"}, Vec3(5,0,90))

lsystem:advance(state)

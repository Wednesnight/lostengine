module("aqua", package.seeall)

local config = require("config")

Physics = {}

function Physics.getGravity()
  return config.physics.gravity or lost.math.Vec2(0, -10)
end

function Physics.getWorldRatio()
  return config.physics.worldRatio or 1.0 / 10.0
end

function Physics.screenToWorldVec2(b2Vec2)
  return box2d.b2Vec2(Physics.screenToWorld(b2Vec2.x, b2Vec2.y))
end

function Physics.screenToWorld(x, y)
  return x * Physics.getWorldRatio(), y * Physics.getWorldRatio()
end

function Physics.worldToScreenVec2(b2Vec2)
  return box2d.b2Vec2(Physics.worldToScreen(b2Vec2.x, b2Vec2.y))
end

function Physics.worldToScreen(x, y)
  return x / Physics.getWorldRatio(), y / Physics.getWorldRatio()
end

module("lost.guiro.themes.default", package.seeall)

--[[
  Renderers
]]
require("lost.guiro.Renderer")
require("lost.guiro.themes.default.renderers.AnimatedImage")
require("lost.guiro.themes.default.renderers.Button")
require("lost.guiro.themes.default.renderers.Image")
require("lost.guiro.themes.default.renderers.Label")
require("lost.guiro.themes.default.renderers.Slider")
require("lost.guiro.themes.default.renderers.Window")

renderers["lost.guiro.View"] = lost.guiro.Renderer
renderers["lost.guiro.Screen"] = lost.guiro.Renderer
renderers["lost.guiro.UserInterface"] = lost.guiro.Renderer

renderers["lost.guiro.controls.AnimatedImage"] = lost.guiro.themes.default.renderers.AnimatedImage
renderers["lost.guiro.controls.Button"] = lost.guiro.themes.default.renderers.Button
renderers["lost.guiro.controls.Image"] = lost.guiro.themes.default.renderers.Image
renderers["lost.guiro.controls.Label"] = lost.guiro.themes.default.renderers.Label
renderers["lost.guiro.controls.Slider"] = lost.guiro.themes.default.renderers.Slider
renderers["lost.guiro.controls.Window"] = lost.guiro.themes.default.renderers.Window

module("lost.guiro.themes.default", package.seeall)

--[[
  Styles
]]
require("lost.guiro.Style")
require("lost.guiro.themes.default.styles.AnimatedImage")
require("lost.guiro.themes.default.styles.Button")
require("lost.guiro.themes.default.styles.Image")
require("lost.guiro.themes.default.styles.Label")
require("lost.guiro.themes.default.styles.Slider")
require("lost.guiro.themes.default.styles.Window")

styles["lost.guiro.View"] = lost.guiro.Style
styles["lost.guiro.Screen"] = lost.guiro.Style
styles["lost.guiro.UserInterface"] = lost.guiro.Style

styles["lost.guiro.controls.AnimatedImage"] = lost.guiro.themes.default.styles.AnimatedImage
styles["lost.guiro.controls.Button"] = lost.guiro.themes.default.styles.Button
styles["lost.guiro.controls.Image"] = lost.guiro.themes.default.styles.Image
styles["lost.guiro.controls.Label"] = lost.guiro.themes.default.styles.Label
styles["lost.guiro.controls.Slider"] = lost.guiro.themes.default.styles.Slider
styles["lost.guiro.controls.Window"] = lost.guiro.themes.default.styles.Window

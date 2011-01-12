-- require this file in startup() of your tasklet if you want to use guiro.
-- You must do this in startup() because a GL context might not exist before startup is called.
-- This is mainly to prevent a circular dependency mess until we figure out a better solution.
-- the order of the requires is relevant so stuff might break if you change it around
require("lost.guiro.TextureManager")
require("lost.guiro.ThemeManager")
require("lost.guiro.WindowManager")
require("lost.guiro.UpdateManager")
require("lost.guiro.Singletons")
lost.guiro.themeManager() -- called to instantiate gradients before they're used in places other than the styles
require("lost.guiro.Bounds")
require("lost.guiro.layout.Vertical")
require("lost.guiro.layout.Horizontal")
require("lost.guiro.layer.Disc")
require("lost.guiro.layer.HLine")
require("lost.guiro.layer.Image")
require("lost.guiro.layer.Layer")
require("lost.guiro.layer.Rect")
require("lost.guiro.layer.RoundedRect")
require("lost.guiro.layer.Text")
require("lost.guiro.view.Box")
require("lost.guiro.view.Button")
require("lost.guiro.view.Label")
require("lost.guiro.view.RadioGroup")
require("lost.guiro.view.TabBar")
require("lost.guiro.view.TabView")
require("lost.guiro.view.UserInterface")
require("lost.guiro.view.View")
require("lost.guiro.view.Window")


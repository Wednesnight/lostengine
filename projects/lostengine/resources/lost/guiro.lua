-- require this file in startup() of your tasklet if you want to use guiro.
-- You must do this in startup() because a GL context might not exist before startup is called.
-- This is mainly to prevent a circular dependency mess until we figure out a better solution.
-- the order of the requires is relevant so stuff might break if you change it around
require("lost.common.CallLater")
require("lost.guiro.TextureManager")
require("lost.guiro.ThemeManager")
require("lost.guiro.WindowManager")
require("lost.guiro.UpdateManager")
require("lost.guiro.Singletons")
-- this is the reason you need to call this in startup(): themeManager might already instantiate some resources that require a GL context
require("lost.guiro.Bounds")
require("lost.guiro.event.Event")
require("lost.guiro.event.EventManager")
require("lost.guiro.layout.Vertical")
require("lost.guiro.layout.Horizontal")
require("lost.guiro.layer.Disc")
require("lost.guiro.layer.HLine")
require("lost.guiro.layer.Image")
require("lost.guiro.layer.Layer")
require("lost.guiro.layer.Rect")
require("lost.guiro.layer.RoundedRect")
require("lost.guiro.layer.Text")
require("lost.guiro.view.View")
require("lost.guiro.view.Box")
require("lost.guiro.view.Button")
require("lost.guiro.view.Image")
require("lost.guiro.view.Label")
require("lost.guiro.view.RadioGroup")
require("lost.guiro.view.TabBar")
require("lost.guiro.view.TabView")
require("lost.guiro.view.UserInterface")
require("lost.guiro.view.Window")
require("lost.guiro.view.Slider")
require("lost.guiro.view.ColorPicker")
require("lost.guiro.view.ColorPickerWindow")
require("lost.guiro.view.FpsMeter")
require("lost.guiro.view.TextInput")
require("lost.guiro.view.MenuItem")
require("lost.guiro.view.Menu")
require("lost.guiro.view.MenuBar")
require("lost.guiro.view.MenuBarItem")
require("lost.guiro.view.PopUpButton")
lost.guiro.themeManager() -- called to instantiate gradients before they're used in places other than the styles

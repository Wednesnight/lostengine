module("lost.guiro.view", package.seeall)
require("lost.guiro.view.View")

lost.common.Class "lost.guiro.view.RadioGroup" "lost.guiro.view.View" {}

function RadioGroup:constructor(args)
	lost.guiro.view.View.constructor(self, args) -- call after aall members that are required for style are setup
  local t = args or {}  
end



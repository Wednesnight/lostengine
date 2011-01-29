module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.RadioGroup" "lost.guiro.view.View" {}

function RadioGroup:constructor(args)
	lost.guiro.view.View.constructor(self, args) -- call after aall members that are required for style are setup
  self:addEventListener("buttonClick", function(event) self:buttonClick(event) end)
  self.id = args.id or "radiogroup"
end

function RadioGroup:buttonClick(event)
  for k,v in ipairs(self.subviews) do
    if v:isDerivedFrom("lost.guiro.view.Button") then
      if v.id ~= event.target.id then
        v:pushed(false)
      end
    end
  end
end

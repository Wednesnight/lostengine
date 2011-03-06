module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.Box" "lost.guiro.view.View" {}

-- a simple box with titleLabel and contentView
-- subviews are added to contentview
function Box:constructor(args)
  lost.guiro.view.View.constructor(self, args)
  local t = args or {}
  self.id = t.id or "box"
  if t.title and self.titleLabel then
    self.titleLabel:text(t.title)
  end
  if t.clip and self.contentView then
    self.contentView:clip(t.clip)
  end

	self._bbcounter = lost.profiler.BBCount("lost.guiro.view.Box")    
end

function Box:addSubview(view,forceSubview)
  if self.contentView and not forceSubview then
    self.contentView:addSubview(view)
  else
    lost.guiro.view.View.addSubview(self, view)
  end
end

function Box:title(...)
  if arg.n >= 1 then 
    self.titleLabel:text(arg[1])
  else
    if self.titleLabel then
      return self.titleLabel:text()
    else
      return nil
    end
  end
end


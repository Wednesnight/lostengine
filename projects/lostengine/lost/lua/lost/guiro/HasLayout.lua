--[[
    lost.guiro.HasLayout
    
    HasLayout is an interface that implements all layout related functionality of a Guiro View.
    Example:

      lost.common.Class "MyLayoutView" "lost.guiro.View" {}
      
      function MyLayoutView:beforeLayout()
        -- update bounds, etc.
      end
      
      function MyLayoutView:afterLayout()
        -- use self.rect for updates, etc.
      end
      
      function MyLayoutView:modifyingMethod()
        -- modify bounds, etc.
        self:needsLayout()
      end
  ]]
module("lost.guiro", package.seeall)

require("lost.common.Class")
require("lost.common.CallLater")

lost.common.Class "lost.guiro.HasLayout" {}

using "lost.common.callLater"

function HasLayout:constructor()
  self.bounds = lost.guiro.Bounds(xabs(0), yabs(0), wabs(0), habs(0))
  self.rect = lost.math.Rect()

  self.dirtyLayout = false
end

--[[
    resize and call needsLayout
  ]]
function HasLayout:resize(bounds)
  self.bounds = bounds
  self:needsLayout()
end

--[[
    Updates dirtyLayout flag and inserts self into layout queue
  ]]
function HasLayout:needsLayout()
  if not self.dirtyLayout then
    self.dirtyLayout = true
    
    -- add to tasklet queue
    callLater(HasLayout._layout, self)
  end
end

--[[
    Internal layout, do not use! See also: HasLayout:beforeLayout(), HasLayout:afterLayout()
  ]]
function HasLayout:_layout(force)
  log.debug(tostring(self) .."(".. self.id .."):_layout()")
  if force or self.dirtyLayout then
    self:beforeLayout()

    self.dirtyLayout = false
    
    -- update rect
    local parentRect = nil
    if self.parent ~= nil then
      parentRect = self.parent.rect
    else
      parentRect = lost.math.Rect()
    end
    self.rect = self.bounds:rect(parentRect)
    log.debug(tostring(self) ..": ".. tostring(self.rect))
    
    -- Call invisible _layout() on all subviews
    if type(self.subviews) == "table" then
      for k,view in next,self.subviews do
        view:_layout(true)
      end
    end

    self:afterLayout()
  end
end

--[[
    Override this method to implement your layout code before rect is updated
  ]]
function HasLayout:beforeLayout()
end

--[[
    Override this method to implement your layout code after rect was updated
  ]]
function HasLayout:afterLayout()
end

--[[
    checks if point is within rect
  ]]
function HasLayout:containsCoord(point)
  return self.rect:contains(point)
end

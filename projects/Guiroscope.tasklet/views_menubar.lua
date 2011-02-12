--[[lost.guiro.ui():addEventListener("menuItemSelected",function(event) 
      log.debug(event.type.." "..event.target.id)
      for k,v in ipairs(event.indexPath) do
        log.debug(v)
      end
      local mi = event.target:menuItemForIndexPath(event.indexPath)
      mi:checked(not mi:checked())
    end
    )]]

local function checkFunction(event) 
  for k,v in ipairs(event.indexPath) do
    log.debug(v)
  end
  -- target can be a popupbutton so we need to check
  if event.target.menuItemForIndexPath then
    local mi = event.target:menuItemForIndexPath(event.indexPath)
    mi:checked(not mi:checked())
  end
end

-- FIXME: menuItemSelected events only through ui
lost.guiro.ui():addEventListener("menuItemSelected", checkFunction)        
        
return lost.guiro.view.View
{
  subviews=
  {
    lost.guiro.view.MenuBar
    {
      bounds={0,"top","1",30},
      items=require("menudef"),
      size="regular",
    },
    lost.guiro.view.MenuBar
    {
      bounds={0,{"top",-50},"1",30},
      items=require("menudef"),
      size="small",
    },
    lost.guiro.view.MenuBar
    {
      bounds={0,{"top",-100},"1",30},
      items=require("menudef"),
      size="mini",
    }    
  }
}
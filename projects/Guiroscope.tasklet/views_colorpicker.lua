local function lv(txt,y)
  local labelWidth = 120
  return lost.guiro.view.View
  {
    id="lineView",
    bounds={0,0,"1",30},
    subviews=
    {
      lost.guiro.view.Label{bounds={0,0,labelWidth,"1"},text=txt,valign="center",halign="left"},
      lost.guiro.view.ColorPicker{bounds={labelWidth,0,{"1",-labelWidth},"1"},name=txt},
    }
  }
end


return lost.guiro.view.View
{
    listeners =
    {
--      mouseEnter = function(event) log.debug("enter "..event.target.id) end,
--      mouseLeave = function(event) log.debug("leave "..event.target.id) end,
--      mouseDown = function(event) log.debug("down "..event.target.id) end,
    },
  subviews=
  {
    lost.guiro.view.Box
    {
      bounds={50,{"top",-50},300,300},
      title = "Colors",
      subviews = 
      {
        lost.guiro.view.View
        {
          id="boxLayoutView",
          layout=lost.guiro.layout.Vertical{valign="top",halign="center",spacing=10},
          subviews = 
          {
            lv("Border Color",20),
            lv("Fill Color",80),
            lv("Pretty Color",140),            
          }
        }
      }
    }
  }
}
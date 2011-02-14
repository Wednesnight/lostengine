local Color = lost.common.Color

return lost.guiro.view.View
{
  listeners=
  {
    menuItemSelected=function(event)  end
  },
  layout=lost.guiro.layout.Vertical{valign="center", halign="center",spacing=10},
  subviews=
  {
    lost.guiro.view.PopUpButton{bounds={0,0,100,30},size="regular",title = "Regular",menu=require("pubmenu1"),selected={1}},
    lost.guiro.view.PopUpButton{bounds={0,0,100,30},size="small",title = "Small",menu=require("pubmenu1"),selected={2}},
    lost.guiro.view.PopUpButton{bounds={0,0,100,30},size="mini",title = "Mini",menu=require("pubmenu1"),selected={3}},
  }
}
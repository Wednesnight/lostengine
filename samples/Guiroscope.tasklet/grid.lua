
function imgcell(name)
  local bmp = lost.bitmap.Bitmap.create(tasklet.loader:load(name))
  bmp:premultiplyAlpha()
  return lost.guiro.view.Image
  {
    bounds={0,0,svc_cellsize,svc_cellsize},
    style="roundCell",
    flip=true,
    bitmap=bmp
  }
end

return function()
return lost.guiro.view.View
{
  bounds={0,0,svc_gridWidth,svc_gridHeight},
  layout=lost.guiro.layout.Vertical{valign="center",halign="center",spacing=svc_cellspacing},
  subviews=
  {
    lost.guiro.view.View
    {
      bounds={0,0,"1",svc_cellsize},
      layout=lost.guiro.layout.Horizontal{valign="center",halign="center",spacing=svc_cellspacing},
      subviews=
      {
        imgcell("img/accept.png"),
        imgcell("img/add.png"),
        imgcell("img/cancel.png"),
        imgcell("img/cross_octagon_fram.png"),
      }
    },
    lost.guiro.view.View
    {
      bounds={0,0,"1",svc_cellsize},      
      layout=lost.guiro.layout.Horizontal{valign="center",halign="center",spacing=svc_cellspacing},
      subviews=
      {
        imgcell("img/cross_octagon.png"),
        imgcell("img/cross_shield_2.png"),
        imgcell("img/cross_shield.png"),
        imgcell("img/cross.png"),
      }
    },
    lost.guiro.view.View
    {
      bounds={0,0,"1",svc_cellsize},    
      layout=lost.guiro.layout.Horizontal{valign="center",halign="center",spacing=svc_cellspacing},
      subviews=
      {
        imgcell("img/delete.png"),
        imgcell("img/emoticon_evilgrin.png"),
        imgcell("img/emoticon_grin.png"),
        imgcell("img/emoticon_happy.png"),
      }
    },
    lost.guiro.view.View
    {
      bounds={0,0,"1",svc_cellsize},    
      layout=lost.guiro.layout.Horizontal{valign="center",halign="center",spacing=svc_cellspacing},
      subviews=
      {
        imgcell("img/emoticon_smile.png"),
        imgcell("img/emoticon_surprised.png"),
        imgcell("img/emoticon_tongue.png"),
        imgcell("img/emoticon_unhappy.png"),
      }
    },
    lost.guiro.view.View
    {
      bounds={0,0,"1",svc_cellsize},    
      layout=lost.guiro.layout.Horizontal{valign="center",halign="center",spacing=svc_cellspacing},
      subviews=
      {
        imgcell("img/emoticon_waii.png"),
        imgcell("img/emoticon_wink.png"),
        imgcell("img/envelope_2.png"),
        imgcell("img/envelope.png"),
      }
    },
  }
}
end


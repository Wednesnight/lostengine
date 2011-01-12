local Color = lost.common.Color

return lost.guiro.view.View
{
  subviews=
  {
    lost.guiro.view.Box
    {
      bounds={20,{"top",-20},200,200},
      title="Box",
      subviews=
      {
        lost.guiro.view.Button
        {
          bounds={0,"top","1",30},
          title="Testing",
        },        
      }
    },    
  }
}
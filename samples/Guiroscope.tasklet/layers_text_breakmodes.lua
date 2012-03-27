local Color = lost.common.Color

local txt = 
[[
Frankenstein,

or the Modern Prometheus


by

Mary Wollstonecraft (Godwin) Shelley




Letter 1


St. Petersburgh, Dec. 11th, 17--

TO Mrs. Saville, England

You will rejoice to hear that no disaster has accompanied the commencement of an enterprise which you have regarded with such evil forebodings.  I arrived here yesterday, and my first task is to assure my dear sister of my welfare and increasing confidence in the success of my undertaking.

I am already far north of London, and as I walk in the streets of Petersburgh, I feel a cold northern breeze play upon my cheeks, which braces my nerves and fills me with delight.  Do you understand this feeling?  This breeze, which has travelled from the regions towards which I am advancing, gives me a foretaste of those icy climes. Inspirited by this wind of promise, my daydreams become more fervent and vivid.
]]

local fnt = {"Vera serif", 14}

return lost.guiro.view.View
{
  id="layers_text_breakmodes",
  bounds = {0,0,"1","1"},
  layout=lost.guiro.layout.Horizontal{valign="center",halign="center",spacing=4},
  subviews=
  {
    lost.guiro.view.Box
    {
      bounds={0,0,".333","1"},
      title="none",
      clip=true,
      subviews=
      {
        lost.guiro.view.Label
        {
          text=txt,
          clip=true,
          color=Color(0,0,0),
          shadow=true,
          shadowColor=Color(.8,1,.8),
          shadowOffset=lost.math.Vec2(0,-1),
          breakMode="none",
          valign="top",
          font = fnt,
        }
      }
    },
    lost.guiro.view.Box
    {
      bounds={0,0,".333","1"},
      title="char",
      clip=true,
      subviews=
      {
        lost.guiro.view.Label
        {
          text=txt,
          clip=true,
          color=Color(0,0,0),
          shadow=true,
          shadowColor=Color(.8,1,.8),
          shadowOffset=lost.math.Vec2(0,-1),
          breakMode="char",
          valign="top",
          font = fnt,          
        }
      }
    },
    lost.guiro.view.Box
    {
      bounds={0,0,".333","1"},
      title="word",
      clip=true,
      subviews=
      {
        lost.guiro.view.Label
        {
          text=txt,
          clip=true,
          color=Color(0,0,0),
          shadow=true,
          shadowColor=Color(.8,1,.8),
          shadowOffset=lost.math.Vec2(0,-1),
          breakMode="word",
          valign="top",
          font = fnt,          
        }
      }
    }    
  }
}


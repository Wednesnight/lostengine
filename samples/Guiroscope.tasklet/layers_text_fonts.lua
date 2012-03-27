local Color = lost.common.Color

local textHeight = 20
local exampleHeight=40

return lost.guiro.view.View
{
  id="layers_text_fonts",
  bounds = {0,0,"1","1"},
  sublayers =
  {
    lost.guiro.layer.Layer
    {      
      bounds={10,10,{"1",-20},{"1",-textHeight}},
      layout=lost.guiro.layout.Vertical{valign="center", halign="center", spacing=10},
      clip = true,
      sublayers=
      {
        lost.guiro.layer.Text{bounds={0,0,"1",exampleHeight},font={"Vera", 12}, text="The quick brown fox jumps over the lazy dog.",color=Color(0,0,0),halign="left"},
        lost.guiro.layer.Text{bounds={0,0,"1",exampleHeight},font={"Vera", 13}, text="The quick brown fox jumps over the lazy dog.",color=Color(0,0,0),halign="right"},
        lost.guiro.layer.Text{bounds={0,0,"1",exampleHeight},font={"Vera bold", 14}, text="The quick brown fox jumps over the lazy dog.",color=Color(0,0,1),halign="center"},
        lost.guiro.layer.Text{bounds={0,0,"1",exampleHeight},font={"Vera bold italic", 15}, text="The quick brown fox jumps over the lazy dog.",color=Color(0,0,0)},
        lost.guiro.layer.Text{bounds={0,0,"1",exampleHeight},font={"Vera italic", 16}, text="The quick brown fox jumps over the lazy dog.",color=Color(0,1,0),valign="top"},
        lost.guiro.layer.Text{bounds={0,0,"1",exampleHeight},font={"Vera mono bold", 17}, text="The quick brown fox jumps over the lazy dog.",color=Color(0,0,0)},
        lost.guiro.layer.Text{bounds={0,0,"1",exampleHeight},font={"Vera mono bold italic", 18}, text="The quick brown fox jumps over the lazy dog.",color=Color(1,0,0),valign="bottom"},
        lost.guiro.layer.Text{bounds={0,0,"1",exampleHeight},font={"Vera mono", 19}, text="The quick brown fox jumps over the lazy dog.",color=Color(0,0,0)},
        lost.guiro.layer.Text{bounds={0,0,"1",exampleHeight},font={"Vera serif", 20}, text="The quick brown fox jumps over the lazy dog.",color=Color(0,0,0)},
        lost.guiro.layer.Text{bounds={0,0,"1",exampleHeight},font={"Vera serif bold", 21}, text="The quick brown fox jumps over the lazy dog.",color=Color(0,0,0)},
        lost.guiro.layer.Text{bounds={0,0,"1",exampleHeight},font={"Grinched", 30}, text="The quick brown fox jumps over the lazy dog.",color=Color(1,.5,.7)},
      }
    }
  }
}


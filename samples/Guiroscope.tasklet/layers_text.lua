local Color = lost.common.Color

local textHeight = 20
local exampleHeight=40

return lost.guiro.view.TabView
{
  id="layers_text",
  bounds = {0,0,"1","1"},
  items=
  {
    {"Fonts",require("layers_text_fonts")},
    {"BreakModes",require("layers_text_breakmodes")},
  }
}


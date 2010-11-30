-- Ivy ui.lua
local Color = lost.common.Color

require("sizes")

local wincol = .1

return dcl.guiro:UserInterface
{
    dcl.guiro:View
    {
      bounds = {0,0,"1", "1"},
      showFrame = false,
      showBackground = true,
      backgroundColor = Color(wincol, wincol, wincol, 1),
      dcl.guiro:View
      {
        bounds = {mainBorder, {"top", -mainBorder}, {"1", -mainBorder*2-toolViewWidth-toolViewSpacing}, {"1", -mainBorder*2-pathLabelHeight-pathLabelSpacing}},
        showFrame = false,
        showBackground = false,
        dcl.guiro:Label
        {
          id = "helpLabel",
          bounds = {"center", "center", 50, 20},
          text = "Drop bitmap here!"
        },
        require("logo"),
        dcl.guiro:View
        {
          bounds = {"left", "bottom", "1", {"1", -taskletIconSize-3}}, 
          showFrame = true,
          frameColor = Color(.6, .6, .6),
          dcl.guiro:Image
          {
            id = "imageView",
            bounds = {"center", "center", "1", "1"}, 
            showFrame = true,
            showBackground = false,
            backgroundColor = Color(1,1,0,1),
            filter = true,
            flip = true
          }
        }
      },
      require("toolView"),
      dcl.guiro:Label
      {
        id = "pathNameLabel",
        bounds = {{"left", mainBorder}, {"bottom", mainBorder}, {"1", -mainBorder*2}, pathLabelHeight}, 
        text = "<no bitmap loaded>",
        showBackground = false,
        showFrame = false
      }      
    }
}
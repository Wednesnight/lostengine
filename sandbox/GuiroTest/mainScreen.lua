local Color = lost.common.Color

guiro:Screen
{
  guiro:UserInterface
  {
    id = "mainUI",

    guiro:Window
    {
      id = "mainMenu",
      style = guiro:Style("lost/guiro/styles/default/Window.lua"),

--[[      guiro:Button
      {
        id = "closeButton",
        bounds = lost.math.Rect(100, 100, 25, 25),
        style =
        {
          color = Color(0,0,0,0)
        }
      }]]--
    }
  }
}

guiro:Screen
{
  id = "mainScreen",
  bounds = lost.math.Rect(0, 0, 800, 600),

  guiro:UserInterface
  {
    id = "mainUI",
    bounds = lost.math.Rect(0, 0, 800, 600),

    guiro:Window
    {
      id = "mainMenu",
      bounds = lost.math.Rect(250, 200, 300, 200),
      style = guiro:Style("lost/guiro/styles/default/Window.lua"),
      renderer = guiro:Renderer("lost/guiro/renderer/default/Window.lua"),

      guiro:Button
      {
        id = "closeButton",
        bounds = lost.math.Rect(240, 10, 50, 25),
        style = guiro:Style("lost/guiro/styles/default/Button.lua"),
        renderer = guiro:Renderer("lost/guiro/renderer/default/Button.lua"),
      }
    }
  }
}

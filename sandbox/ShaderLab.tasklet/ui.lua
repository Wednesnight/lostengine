require("lost.guiro")

local Color = lost.common.Color

return lost.guiro.view.View
{
  subviews =
  {
    lost.guiro.view.View
    {
      id = "toolbar",
      bounds = {"left", "top", "1", 40},
      subviews =
      {
        lost.guiro.view.Label
        {
          bounds = {10, "top", 100, 25},
          font = {"Vera", 12},
          halign="left",
          text = "LostEngine"
        },
        lost.guiro.view.Label
        {
          bounds = {15, {"top", -15}, 100, 25},
          font = {"Vera", 16},
          halign="left",
          text = "Shader Lab"
        }
      }
    },
    lost.guiro.view.View
    {
      id = "content",
      bounds = {{"left", 10}, {"bottom", 10}, "1", {"1", -50}},
      subviews =
      {
        lost.guiro.view.View
        {
          id = "shaders",
          bounds = {"left", "top", ".5", "1"},
          subviews =
          {
            lost.guiro.view.TextInput
            {
              id = "vs",
              bounds = {"left", "top", "1", ".5"},
              font = {"Vera mono", 10},
              multiLine = true,
              halign = "left",
              valign = "top",
              text =
[[uniform mat4 modelViewMatrix;  // mesh transform
uniform mat4 projectionMatrix; // from camera
uniform vec4 color; // same semantics as gl_Color

attribute vec3 position;

void main(void)
{
  vec4 pos = vec4(position, 1.0);
  gl_Position = projectionMatrix*modelViewMatrix*pos; // equivalent to builtin function ftransform()
}

]]
            },
            lost.guiro.view.TextInput
            {
              id = "fs",
              bounds = {"left", {"top", "-.5"}, "1", ".5"},
              font = {"Vera mono", 10},
              multiLine = true,
              halign = "left",
              valign = "top",
              text =
[[uniform vec4 color; // same semantics as gl_Color

void main(void)
{
  gl_FragColor = color;
}

]]
            }
          }
        }
      }
    }
  }
}

return Shader:declare
{
  include =
  {
    Shader:load("lost.shaders.Light"),
    Shader:load("lost.shaders.Ssao"),
    Shader:load("lost.shaders.MatCap"),
    Shader:load("lost.shaders.ShadowMap")
  },

  vertex = Shader:source
  [[
    // flag for direct lighting / ssao / shadowmap
    uniform bool lightingEnabled;
    uniform bool shadowmapEnabled;
    uniform bool ssaoEnabled;
    uniform bool matcapEnabled;

    // from camera
    uniform mat4 projectionMatrix;

    // mesh transform
    uniform mat4 modelViewMatrix;

    // vertex attributes
    attribute vec3 position;

    void main(void)
    {
      // equivalent to builtin function ftransform()
      gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);

      if (lightingEnabled) processLight();
      if (ssaoEnabled) processSsao();
      if (shadowmapEnabled) processShadowMap();
      if (matcapEnabled) processMatCap();
    }
  ]],

  fragment = Shader:source
  [[
    uniform bool lightingEnabled;
    uniform bool shadowmapEnabled;
    uniform bool ssaoEnabled;
    uniform bool matcapEnabled;

    uniform sampler2DShadow texture0;
    uniform sampler2D texture1;
    uniform sampler2D texture2;
    uniform vec4 color;

    void main(void)
    {
      vec4 col = color;

      if (lightingEnabled) col = processLight(col);
      if (shadowmapEnabled) col = processShadowMap(col, texture0);
      if (ssaoEnabled) col = processSsao(col, texture1);
      if (matcapEnabled) col = processMatCap(col, texture2);

      gl_FragColor = col;
    }
  ]]
}

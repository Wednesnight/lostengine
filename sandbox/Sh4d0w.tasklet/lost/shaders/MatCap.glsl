return Shader:declare
{
  export =
  {
    vertex =
    [[
      void processMatCap(void);
    ]],

    fragment =
    [[
      vec4 processMatCap(in vec4 color, in sampler2D tex);
    ]]
  },

  vertex = Shader:source
  [[
    // constant matrix used to scale texture coordinates
    const mat4 biasMatrix = mat4(vec4(0.5, 0.0, 0.0, 0.0),
                                 vec4(0.0, 0.5, 0.0, 0.0),
                                 vec4(0.0, 0.0, 0.5, 0.0),
                                 vec4(0.5, 0.5, 0.5, 1.0));

   uniform mat4 modelViewMatrix;

   attribute vec3 normal;

   varying vec4 matcapTexCoord;

    void processMatCap(void)
    {
      matcapTexCoord = biasMatrix * normalize(modelViewMatrix * vec4(normal, 0.0));
    }
  ]],

  fragment = Shader:source
  [[
    varying vec4 matcapTexCoord;

    vec4 processMatCap(in vec4 color, in sampler2D tex)
    {
      return color * texture2D(tex, matcapTexCoord.xy);
    }
  ]]
}

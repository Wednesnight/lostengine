return Shader:declare
{
  export =
  {
    vertex =
    [[
      void processShadowMap(void);
    ]],

    fragment =
    [[
      vec4 processShadowMap(in vec4 color, in sampler2DShadow tex);
    ]]
  },

  vertex = Shader:source
  [[
    // constant matrix used to scale texture coordinates
    const mat4 biasMatrix = mat4(vec4(0.5, 0.0, 0.0, 0.0),
                                 vec4(0.0, 0.5, 0.0, 0.0),
                                 vec4(0.0, 0.0, 0.5, 0.0),
                                 vec4(0.5, 0.5, 0.5, 1.0));

    // mesh transform
    uniform mat4 modelViewMatrix;

    // from light cam
    uniform mat4 lightViewMatrix;
    uniform mat4 lightProjectionMatrix;

    attribute vec3 position;

    varying vec4 shadowTexCoord;

    void processShadowMap(void)
    {
      shadowTexCoord = biasMatrix * lightProjectionMatrix * lightViewMatrix * modelViewMatrix * vec4(position, 1.0);
    }
  ]],

  fragment = Shader:source
  [[
    varying vec4 shadowTexCoord;

    vec4 processShadowMap(in vec4 color, in sampler2DShadow tex)
    {	
      vec4 result = color;
      vec3 s = vec3(shadowTexCoord.xyz / shadowTexCoord.w);
      if (s.x >= 0.0 && s.x <= 1.0 && s.y >= 0.0 && s.y <= 1.0)
      {
        float shadowMapDepth = shadow2D(tex, s).z;
        if (shadowMapDepth < s.z - 0.005)
        {
          result = vec4(0.6 * color.rgb, color.a);
        }
      }
      return result;
    }
  ]]
}

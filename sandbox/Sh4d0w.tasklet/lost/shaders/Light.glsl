return Shader:declare
{
  export =
  {
    vertex =
    [[
      void processLight(void);
    ]],
    
    fragment =
    [[
      vec4 processLight(in vec4 color);
    ]]
  },

  vertex = Shader:source
  [[
    // mesh transform
    uniform mat4 modelViewMatrix;

    // from light cam
    uniform vec3 lightPosition;

    // vertex attributes
    attribute vec3 position;
    attribute vec3 normal;

    // will be calculated per vertex and interpolated per fragment
    varying float lightIntensity;

    float calculateLightIntensity(in vec4 pos, in vec4 normal, in vec3 lightPosition)
    {
      vec3 ecpos = vec3(modelViewMatrix * pos);
      vec3 lightVec = normalize(lightPosition - ecpos);
      vec3 tnorm = vec3(normalize(modelViewMatrix * normal));
      return max(dot(lightVec, tnorm), 0.0);
    }

    void processLight(void)
    {
      lightIntensity = calculateLightIntensity(vec4(position, 1.0), vec4(normal, 0.0), lightPosition);
    }
  ]],

  fragment = Shader:source
  [[
    // will be calculated per vertex and interpolated per fragment
    varying float lightIntensity;

    vec4 processLight(in vec4 color)
    {
      return vec4(lightIntensity * color.rgb, color.a);
    }
  ]]
}

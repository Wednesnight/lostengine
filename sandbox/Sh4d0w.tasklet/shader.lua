return
{
  vertexShader =
  [[
  float calculateLightIntensity(in vec4 pos, in vec4 normal, in mat4 modelViewMatrix, in vec3 lightPosition);

  // constant matrix used to scale shadowTexCoord to texture coordinates
  uniform mat4 biasMatrix;

  // from light cam
  uniform vec3 lightPosition;
  uniform mat4 lightViewMatrix;
  uniform mat4 lightProjectionMatrix;

  // mesh transform
  uniform mat4 modelViewMatrix;

  // from camera
  uniform mat4 projectionMatrix;

  // flag for direct lighting / ssao / shadowmap
  uniform bool lightingEnabled;
  uniform bool ssaoEnabled;
  uniform bool shadowmapEnabled;
  uniform bool matcapEnabled;

  attribute vec3 position;
  attribute vec3 normal;

  // will be calculated per vertex and interpolated per fragment
  varying float lightIntensity;

  varying vec4 ssaoTexcoord;
  varying vec4 shadowTexCoord;
  varying vec4 matcapTexCoord;

  void main(void)
  {
    vec4 pos = vec4(position, 1.0);

    // equivalent to builtin function ftransform()
    gl_Position = projectionMatrix*modelViewMatrix*pos;

    if (lightingEnabled)
    {
      lightIntensity = calculateLightIntensity(pos, vec4(normal, 0.0), modelViewMatrix, lightPosition);
    }

    if (ssaoEnabled)
    {
      ssaoTexcoord = biasMatrix * gl_Position;
    }

    /*
     * shadow projection
     */
    if (shadowmapEnabled)
    {
      shadowTexCoord = biasMatrix * lightProjectionMatrix * lightViewMatrix * modelViewMatrix * pos;
    }

    /*
     * calculate matcap coord
     */
    if (matcapEnabled)
    {
      matcapTexCoord = biasMatrix * normalize(modelViewMatrix * vec4(normal, 0.0));
    }
  }
  ]],
  fragmentShader =
  [[
  float calculateSsaoFactor(in vec4 ssaoTexcoord, in sampler2D tex);

  uniform sampler2DShadow texture0;
  uniform sampler2D texture1;
  uniform sampler2D texture2;
  uniform vec4 color;

  uniform bool lightingEnabled;
  uniform bool ssaoEnabled;
  uniform bool shadowmapEnabled;
  uniform bool matcapEnabled;

  // will be calculated per vertex and interpolated per fragment
  varying float lightIntensity;

  varying vec4 ssaoTexcoord;
  varying vec4 shadowTexCoord;
  varying vec4 matcapTexCoord;

  void main(void)
  {	
    float shadow = 1.0;
    if (shadowmapEnabled)
    {
      vec3 s = vec3(shadowTexCoord.xyz / shadowTexCoord.w);
      if (s.x >= 0.0 && s.x <= 1.0 && s.y >= 0.0 && s.y <= 1.0)
      {
        float shadowMapDepth = shadow2D(texture0, s).z;
        if (shadowMapDepth < s.z - 0.005)
        {
          shadow = 0.6;
        }
      }
    }

    vec4 shadowedColor;
    if (matcapEnabled)
    {
      vec4 matcapColor = texture2D(texture2, matcapTexCoord.xy);
      shadowedColor = vec4(shadow * color.rgb * matcapColor.rgb, color.a * matcapColor.a);
    }
    else
    {
      shadowedColor = vec4(shadow * color.rgb, color.a);
    }

  	float ao = 0.0;
    if (ssaoEnabled)
    {
      ao = calculateSsaoFactor(ssaoTexcoord, texture1);
    }
    gl_FragColor = vec4(1.0 - ao) * ((lightingEnabled) ? lightIntensity * shadowedColor : shadowedColor);
  }
  ]]
}

uniform vec4 color;
uniform sampler2DShadow texture0;

// will be calculated per vertex and interpolated per fragment
varying float lightIntensity;

// shadow map params
varying vec4 shadowTexCoord;

void main(void)
{
  float shadow = 1.0;

  vec3 s = vec3(shadowTexCoord.xyz / shadowTexCoord.w);
  if (s.x >= 0.0 && s.x <= 1.0 && s.y >= 0.0 && s.y <= 1.0)
  {
    float depth = shadow2D(texture0, s).z;
    if (depth < s.z - 0.005)
    {
      shadow = 0.2;
    }
  }

  gl_FragColor = vec4(shadow * color.rgb * lightIntensity, color.a);
}

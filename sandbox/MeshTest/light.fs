uniform vec3 LightPosition;
uniform vec4 LightDiffuse;
uniform vec4 LightAmbient;
uniform vec4 LightSpecular;

varying vec3 normal;
varying vec4 vertex;

void main(void)
{
  /**
   * fragment vectors
   */
  vec3 fragNormal = normalize(normal);
  vec3 fragLight = normalize(LightPosition - vertex.xyz);
  vec3 fragView = -normalize(vertex.xyz);
  vec3 fragHalf = normalize(fragLight + fragView);

  /**
   * diffuse
   */
  vec4 diffuse = max(0.0, dot(fragLight, fragNormal)) * gl_FrontMaterial.diffuse * normalize(LightDiffuse + gl_Color);

  /**
   * ambient
   */
  vec4 ambient = gl_FrontMaterial.ambient * normalize(LightAmbient + gl_Color);

  /**
   * specular
   */
  vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);
  if (dot(fragLight, fragView) > 0.0)
  {
    specular = pow(max(0.0, dot(fragNormal, fragHalf)), gl_FrontMaterial.shininess) * gl_FrontMaterial.specular * normalize(LightSpecular + gl_Color);
  }

  vec3 color = clamp(vec3(diffuse.rgb + ambient.rgb + specular.rgb), 0.0, 1.0);
  gl_FragColor = vec4(color, 1.0);
}

varying vec3 DiffuseColor;
varying vec3 SpecularColor;
uniform vec3 SurfaceColor;

void main(void)
{ 
  vec3 finalColor = SurfaceColor * DiffuseColor + SpecularColor;
  gl_FragColor = vec4(finalColor, 1.0);
}
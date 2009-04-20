uniform vec3 LightPosition;

void main(void)
{
  /**
   * float theta = dot(gl_Normal, normalize(LightPosition));
   */
  gl_FrontColor = vec4(dot(gl_Normal, normalize(LightPosition)) * gl_Color.rgb, gl_Color.a);
  gl_Position = ftransform();
}

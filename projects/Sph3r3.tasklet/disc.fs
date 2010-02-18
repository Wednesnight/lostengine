varying float lightIntensity;

void main(void)
{
  vec2 c = vec2(.5, .5);
  float r = gl_TexCoord[0].z;
  float border = .01;
  float srstart = r-border;
  vec2 p = (gl_TexCoord[0]-vec4(c, 0, 0)).xy;
  float currentR = sqrt(p.x*p.x+p.y*p.y);
  float v = 0.0;
  if(currentR < srstart)
  {
    v = 1.0;
  }
  else if(currentR <=r)
  {
    v = 1.0-smoothstep(srstart, r, currentR);
  }
  float f = v;
  gl_FragColor = vec4(gl_Color.rgb*lightIntensity,f);
//gl_FragColor = vec4(f,f,f,1);
}
varying float lightIntensity;
varying vec3 texcoord;
uniform vec4 color; // same semantics as gl_Color

void main(void)
{
  vec2 c = vec2(.5, .5);
  float r = texcoord.z;
  float border = .01;
  float srstart = r-border;
  vec2 p = (texcoord-vec3(c, 0)).xy;
  float currentR = sqrt(p.x*p.x+p.y*p.y);
  float v = 0.0;
  v=step(currentR, srstart);
  if(v <= 0.0) discard;
  /*if(currentR < srstart)
  {
    v = 1.0;
  }
  else if(currentR <=r)
  {
    v = 1.0-smoothstep(srstart, r, currentR);
  }
  if(f <= 0.0) discard;*/
  float f = v;
  gl_FragColor = vec4(color.rgb*lightIntensity,f);
//gl_FragColor = vec4(f,f,f,1);
}
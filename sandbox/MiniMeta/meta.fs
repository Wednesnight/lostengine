uniform vec4 color; // same semantics as gl_Color
varying vec3 va0;

float mb(vec3 pos, float rad, vec3 current)
{
  vec3 tmp = pow(current-pos,vec3(2.));
  return rad/(tmp.x+tmp.y+tmp.z);
}

void main(void)
{
  float rad1 = .1;
  vec3 pos1 = vec3(.2,.2,0);

  float rad2 = .045;
  vec3 pos2 = vec3(.7,.7,0);

  float f = pow(mb(pos1,rad1,va0)+mb(pos2,rad2,va0),80.0);
  gl_FragColor = color*f;
}

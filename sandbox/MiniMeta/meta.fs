uniform vec4 color; // same semantics as gl_Color
varying vec3 va0;

float mb(vec3 pos, float rad, vec3 current)
{
  return rad/(pow(current.x-pos.x,2.0)+pow(current.y-pos.y,2.0)+pow(current.z-pos.z,2.0));
}

void main(void)
{
    float rad1 = .1;
    vec3 pos1 = vec3(.2,.2,0);

    float rad2 = .05;
    vec3 pos2 = vec3(.7,.7,0);

    float thresh = .96;
//    float f = step(.96,mb(vec3(.7,.7,0),.4,va0))
    float f = pow(mb(pos1,rad1,va0)+mb(pos2,rad2,va0),64.0);

//    float mbv = clamp(mb(pos1,rad1,va0)+mb(pos2,rad2,va0),0.0,1.0);
//    float fac = 32.0;
//    float f = 1.0-pow(1.0-pow(mbv,fac),fac);

//    float f = step(thresh,mbv);
//  float f = step(thresh,mb(pos1,rad1,va0));
  gl_FragColor = color*f;
}

uniform vec4 color; // same semantics as gl_Color
varying vec3 va0;
varying vec4 nv;
varying vec4 pos;

float mb(vec3 pos, float rad, vec3 current)
{
  vec3 tmp = pow(current-pos,vec3(2.));
  return rad/(tmp.x+tmp.y+tmp.z);
}

void main(void)
{
  float rad1 = .2;
  vec3 pos1 = vec3(.2,.2,.2);

  float rad2 = .045;
  vec3 pos2 = vec3(.7,.7,.7);

  float f = pow(mb(pos1,rad1,va0)+mb(pos2,rad2,va0),80.0);
  vec4 col = color*f;
  if(col.a < .5) discard;
  col.a = 1.0;
  
  vec4 lightpos = vec4(50,50,50,1.0);
//  vec4 lightcol = vec4(1,1,1,1);
  vec4 lightvec = normalize(lightpos-pos);
  float ambience = 0.0;
  float light = clamp(max(dot(abs(nv),lightvec),0.0)+ambience,0.,1.);
  
//  gl_FragColor = vec4(1.,1.,1.,1.)*light; //col*light; //vec4(col.rgb/(gl_FragCoord.z*5.0),1.0); //vec4(1,0,0,col.a);
  gl_FragColor = clamp(col,0.,1.)*light; //vec4(col.rgb/(gl_FragCoord.z*5.0),1.0); //vec4(1,0,0,col.a);
}

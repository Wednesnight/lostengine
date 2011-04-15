uniform highp vec4 color; // same semantics as gl_Color
uniform highp float drama;
uniform highp float ambience;

uniform highp vec3 pos1;
uniform highp float rad1;
uniform highp vec3 pos2;
uniform highp float rad2;

varying highp vec3 va0;
varying highp vec4 nv;
varying highp vec4 pos;

highp float mb(highp vec3 pos, highp float rad, highp vec3 current)
{
  highp vec3 tmp = pow(current-pos,vec3(2.));
  return rad/(tmp.x+tmp.y+tmp.z);
}

void main(void)
{
  highp float f = pow(mb(pos1,rad1,va0)+mb(pos2,rad2,va0),drama);
  highp vec4 col = color*f;
  if(col.a < .5) discard;
  col.a = 1.0;
  
  lowp vec4 lightpos = vec4(50,50,300,1.0);
//  vec4 lightcol = vec4(1,1,1,1);
  lowp vec4 lightvec = normalize(lightpos-pos);
  lowp float light = clamp(max(dot(abs(nv),lightvec),0.0)+ambience,0.,1.);
  
//  gl_FragColor = vec4(1.,1.,1.,1.)*light; //col*light; //vec4(col.rgb/(gl_FragCoord.z*5.0),1.0); //vec4(1,0,0,col.a);
  gl_FragColor = clamp(col,0.,1.)*light; //vec4(col.rgb/(gl_FragCoord.z*5.0),1.0); //vec4(1,0,0,col.a);
}

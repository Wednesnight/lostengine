varying highp float lightIntensity;
varying highp vec3 texcoord;
uniform highp vec4 color; // same semantics as gl_Color

void main(void)
{
  highp vec2 c = vec2(.5, .5);
  highp float r = texcoord.z;
  highp float border = .01;
  highp float srstart = r-border;
  highp vec2 p = (texcoord-vec3(c, 0)).xy;
  highp float currentR = sqrt(p.x*p.x+p.y*p.y);
  highp float v = 0.0;
  v=step(currentR, srstart);
  
/*  if(currentR < srstart)
  {
    v = 1.0;
  }
  else
    discard;*/
/*  else if(currentR <=r)
  {
    v = 1.0-smoothstep(srstart, r, currentR);
  }*/
  highp float f = v;
  gl_FragColor = vec4(color.rgb*lightIntensity,f);
//gl_FragColor = vec4(f,f,f,1);
}
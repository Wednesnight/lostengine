varying vec2 texcoord;
uniform sampler2D  texture0;
varying float myass;

void main()
{
  vec4 sum = vec4(0);
  int j;
  int i;

  for( i= -4 ;i < 4; i++)
  {
    for (j = -3; j < 3; j++)
    {
      sum += texture2D(texture0, texcoord + vec2(j, i)*0.004) * 0.25;
    }
  }
  if (texture2D(texture0, texcoord).r < 0.3)
  {
    gl_FragColor = sum*sum*0.012 + texture2D(texture0, texcoord);
  }
  else
  {
    if (texture2D(texture0, texcoord).r < 0.5)
    {
      gl_FragColor = sum*sum*0.009 + texture2D(texture0, texcoord);
    }
    else
    {
      gl_FragColor = sum*sum*0.0075 + texture2D(texture0, texcoord);
    }
  }
  gl_FragColor = vec4(gl_FragColor.xyz, step(mod(myass, 2.0), 1.0));
}

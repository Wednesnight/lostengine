uniform sampler2D sampler;
uniform float shader;
uniform float factor;
uniform vec2 pos;

void main(void)
{
	vec2 texCoord = gl_TexCoord[0].xy; // [0.0 ,1.0] x [0.0, 1.0]
	vec2 normCoord = 2.0 * texCoord - pos; // [-1.0 ,1.0] x [-1.0, 1.0]
	float r = length(normCoord); // to polar coords
	float phi = atan(normCoord.y, normCoord.x); // to polar coords
  if (shader == 1.0) r = pow(r, 1.0/1.8) * 0.8;	// squeeze
  if (shader == 2.0) phi = phi + (1.0 - smoothstep(-0.5, 0.5, r)) * factor; // twirl
  if (shader == 3.0) if (r > 0.5) r = 0.5; // light tunnel
  if (shader == 4.0) r = r * smoothstep(-0.1, 0.5, r); // bulge
  if (shader == 5.0) r = 2.0 * r -  r * smoothstep(0.0, 0.7, r); // dent
  if (shader == 6.0) r = r * r / sqrt(2.0); // fish eye
  if (shader == 7.0) // stretch
  {
    vec2 s = sign(normCoord);
    normCoord = abs(normCoord);
    normCoord = 0.5 * normCoord + 0.5 * smoothstep(0.25, 0.5, normCoord) * normCoord;
    normCoord = s * normCoord;
  }
  if (shader < 7.0) // back to cartesian coords
  {
    normCoord.x = r * cos(phi);
    normCoord.y = r * sin(phi);
  }
  if (shader == 8.0) normCoord.x = normCoord.x * sign(normCoord.x); // mirror

	texCoord = normCoord / 2.0 + pos / 2.0; // [0.0 ,1.0] x [0.0, 1.0]

	gl_FragColor = texture2D(sampler, texCoord);
}

uniform sampler2DShadow texture0;
uniform sampler2D texture1;
uniform vec4 color;

uniform vec2 depth;
uniform vec2 viewport;

// will be calculated per vertex and interpolated per fragment
varying float lightIntensity;

varying vec4 ssaoTexcoord;
varying vec4 shadowTexCoord;

float readDepth( in vec2 coord )
{
	return (2.0 * depth.x) / (depth.y + depth.x - texture2D(texture1, coord).x * (depth.y - depth.x));	
}


float compareDepths( in float depth1, in float depth2 )
{
	float aoCap = 1.0;
	float aoMultiplier=10000.0;
	float depthTolerance=0.000;
	float aorange = 10.0;// units in space the AO effect extends to (this gets divided by the camera far range
	float diff = sqrt( clamp(1.0-(depth1-depth2) / (aorange/(depth.y-depth.x)),0.0,1.0) );
	float ao = min(aoCap,max(0.0,depth1-depth2-depthTolerance) * aoMultiplier) * diff;
	return ao;
}


void main(void)
{	
  float shadow = 1.0;

  vec3 s = vec3(shadowTexCoord.xyz / shadowTexCoord.w);
  if (s.x >= 0.0 && s.x <= 1.0 && s.y >= 0.0 && s.y <= 1.0)
  {
    float shadowMapDepth = shadow2D(texture0, s).z;
    if (shadowMapDepth < s.z - 0.005)
    {
      shadow = 0.2;
    }
  }
  vec4 shadowedColor = vec4(shadow * color.rgb * lightIntensity, color.a);

  vec2 texcoord = ssaoTexcoord.xy / ssaoTexcoord.w;

	float ssaoDepth = readDepth(texcoord);
	float d;
	
	float pw = 1.0 / viewport.x;
	float ph = 1.0 / viewport.y;

	float aoCap = 1.0;

	float ao = 0.0;

	float aoMultiplier=10000.0;

	float depthTolerance = 0.001;
	
	float aoscale=1.0;

	d=readDepth( vec2(texcoord.x+pw,texcoord.y+ph));
	ao+=compareDepths(ssaoDepth,d)/aoscale;

	d=readDepth( vec2(texcoord.x-pw,texcoord.y+ph));
	ao+=compareDepths(ssaoDepth,d)/aoscale;

	d=readDepth( vec2(texcoord.x+pw,texcoord.y-ph));
	ao+=compareDepths(ssaoDepth,d)/aoscale;

	d=readDepth( vec2(texcoord.x-pw,texcoord.y-ph));
	ao+=compareDepths(ssaoDepth,d)/aoscale;
	
	pw*=2.0;
	ph*=2.0;
	aoMultiplier/=2.0;
	aoscale*=1.2;
	
	d=readDepth( vec2(texcoord.x+pw,texcoord.y+ph));
	ao+=compareDepths(ssaoDepth,d)/aoscale;

	d=readDepth( vec2(texcoord.x-pw,texcoord.y+ph));
	ao+=compareDepths(ssaoDepth,d)/aoscale;

	d=readDepth( vec2(texcoord.x+pw,texcoord.y-ph));
	ao+=compareDepths(ssaoDepth,d)/aoscale;

	d=readDepth( vec2(texcoord.x-pw,texcoord.y-ph));
	ao+=compareDepths(ssaoDepth,d)/aoscale;

	pw*=2.0;
	ph*=2.0;
	aoMultiplier/=2.0;
	aoscale*=1.2;
	
	d=readDepth( vec2(texcoord.x+pw,texcoord.y+ph));
	ao+=compareDepths(ssaoDepth,d)/aoscale;

	d=readDepth( vec2(texcoord.x-pw,texcoord.y+ph));
	ao+=compareDepths(ssaoDepth,d)/aoscale;

	d=readDepth( vec2(texcoord.x+pw,texcoord.y-ph));
	ao+=compareDepths(ssaoDepth,d)/aoscale;

	d=readDepth( vec2(texcoord.x-pw,texcoord.y-ph));
	ao+=compareDepths(ssaoDepth,d)/aoscale;
	
	pw*=2.0;
	ph*=2.0;
	aoMultiplier/=2.0;
	aoscale*=1.2;
	
	d=readDepth( vec2(texcoord.x+pw,texcoord.y+ph));
	ao+=compareDepths(ssaoDepth,d)/aoscale;

	d=readDepth( vec2(texcoord.x-pw,texcoord.y+ph));
	ao+=compareDepths(ssaoDepth,d)/aoscale;

	d=readDepth( vec2(texcoord.x+pw,texcoord.y-ph));
	ao+=compareDepths(ssaoDepth,d)/aoscale;

	d=readDepth( vec2(texcoord.x-pw,texcoord.y-ph));
	ao+=compareDepths(ssaoDepth,d)/aoscale;

	ao/=16.0;

	gl_FragColor = vec4(1.0-ao) * shadowedColor;
}
